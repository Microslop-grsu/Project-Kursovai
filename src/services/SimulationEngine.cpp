#include "../../include/shelter/services/SimulationEngine.h"
#include "../../include/shelter/services/DietCalculator.h"
#include "../../include/shelter/services/ShelterManager.h"
#include "../../include/shelter/services/SmartBowl.h"
#include "../../include/shelter/storage/json.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

using json = nlohmann::json;

SimulationEngine::SimulationEngine(
    std::shared_ptr<ShelterManager> manager,
    const SimulationConfig& config
) : manager(std::move(manager)),
    eventGenerator(std::make_shared<EventGenerator>(config)),
    vetInspector(std::make_shared<VetInspector>()),
    eventProcessor(std::make_shared<EventProcessor>(this->manager, vetInspector)),
    config(config) {}

void SimulationEngine::run(int totalTicks, int displayInterval) {
    if (totalTicks <= 0) {
        return;
    }

    statusDisplayInterval = std::max(1, displayInterval);
    logEvent("Simulation started.");

    for (int tickIndex = 0; tickIndex < totalTicks; ++tickIndex) {
        tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (currentTick % statusDisplayInterval == 0) {
            displayStatus();
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    if (currentTick % statusDisplayInterval != 0) {
        displayStatus();
    }

    logEvent("Simulation finished.");
}

void SimulationEngine::tick() {
    ++currentTick;
    lastEvents.clear();

    bowlRegistry.sync(manager->getAllPets(), manager->getMedicalRecord());

    if (currentTick % ticksPerDay == 0) {
        bowlRegistry.resetAll();
        logEvent("SmartBowl: daily counters reset for all bowls.");
    }

    updateAnimalStates();
    performHungerCheck();

    const auto generatedEvents = eventGenerator->generateEvents(currentTick, manager->getAllPets());
    for (const auto& event : generatedEvents) {
        if (!eventProcessor->processEvent(event)) {
            logEvent("Failed to process " + eventTypeToString(event->getType()) + ": " + eventProcessor->getLastError());
            continue;
        }

        updateStatisticsForEvent(*event);
        logEvent(event->toString());
    }

    if (config.periodicVetInterval > 0 && currentTick % config.periodicVetInterval == 0) {
        performPeriodicVetCheck();
    }

    capturePopulationSnapshot();
}

int SimulationEngine::getCurrentTick() const {
    return currentTick;
}

std::vector<std::string> SimulationEngine::getLastEvents() const {
    return lastEvents;
}

static std::string healthColor(double value) {
    if (value >= 70.0) return "\033[1;32m";   // Зелёный — здоров
    if (value >= 30.0) return "\033[1;33m";   // Жёлтый — средне
    return "\033[1;31m";                       // Красный — критично
}

std::string SimulationEngine::getShelterStatus() const {
    const auto animals = manager->getAllPets();
    const size_t count = animals.size();

    double totalHunger = 0.0, totalHealth = 0.0;
    size_t critical = 0;

    for (const Pet* pet : animals) {
        totalHunger += pet->getHungerLevel();
        totalHealth += pet->getHealthLevel();
        if (pet->getHealthLevel() < 25.0) critical++;
    }

    const double avgH = count ? totalHunger / count : 0.0;
    const double avgHP = count ? totalHealth / count : 0.0;

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1);
    const std::string R = "\033[0m";

    ss << "\n  .:~ " << "SHELTER REPORT — TICK " << currentTick << " ~:.\n\n";

    ss << "  Pets: " << count
       << "  |  Critical: " << critical
       << "  |  Avg Hunger: " << avgH << "%"
       << "  |  Avg Health: " << avgHP << "%\n";
    ss << "  " << std::string(62, '-') << "\n";

    if (!animals.empty()) {
        ss << "  " << std::left
           << std::setw(6)  << "ID"
           << std::setw(14) << "Name"
           << std::setw(12) << "Hunger"
           << std::setw(14) << "Health"
           << "Status\n";
        ss << "  " << std::string(62, '-') << "\n";

        for (const Pet* pet : animals) {
            const double hun = pet->getHungerLevel();
            const double hp  = pet->getHealthLevel();
            const std::string status = (hp <= 30.0) ? "!!! CRITICAL" :
                                       (hun > 75.0) ? "... HUNGRY" : "OK";

            ss << "  " << std::left
               << std::setw(6)  << pet->getId()
               << std::setw(14) << pet->getName()
               << std::setw(10) << (std::to_string(int(hun)) + "%")
               << std::setw(2)  << ""
               << healthColor(hp) << std::setw(10) << (std::to_string(int(hp)) + "%") << R
               << std::setw(2)  << ""
               << status << "\n";
        }
        ss << "  " << std::string(62, '-') << "\n";
    }
    return ss.str();
}

void SimulationEngine::saveStatistics(const std::string& filename) const {
    const std::filesystem::path path(filename);
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    json statistics = {
        {"totalTicks", currentTick},
        {"animalsArrived", stats.animalsArrived},
        {"animalsAdopted", stats.animalsAdopted},
        {"vaccinationsGiven", stats.vaccinationsGiven},
        {"vetChecksPerformed", stats.vetChecksPerformed},
        {"feedingOccurrences", stats.feedingOccurrences},
        {"criticalHealthCases", stats.criticalHealthCases},
        {"averageHungerLevel", stats.samples == 0 ? 0.0 : stats.accumulatedAverageHunger / stats.samples},
        {"averageHealthLevel", stats.samples == 0 ? 0.0 : stats.accumulatedAverageHealth / stats.samples},
        {"currentPopulation", manager->getPetCount()}
    };

    std::ofstream output(path);
    output << statistics.dump(4);
}

void SimulationEngine::updateAnimalStates() {
    for (Pet* pet : manager->getAllPets()) {
        if (!pet) {
            continue;
        }

        pet->increaseHunger(config.hungerIncreasePerTick);
        if (pet->getHungerLevel() > config.criticalHungerThreshold) {
            const int previousHealth = pet->getHealthLevel();
            pet->changeHealth(-config.healthPenaltyPerCriticalTick);
            if (previousHealth >= 30 && pet->getHealthLevel() < 30) {
                ++stats.criticalHealthCases;
            }

            HealthUpdateEvent healthUpdate(
                pet->getId(),
                pet->getName(),
                previousHealth,
                pet->getHealthLevel(),
                "critical hunger"
            );
            logEvent(healthUpdate.toString());
        }
    }
}

void SimulationEngine::performHungerCheck() {
    for (Pet* pet : manager->getAllPets()) {
        if (!pet || pet->getHungerLevel() <= config.hungerFeedingThreshold) {
            continue;
        }

        const int previousHunger = pet->getHungerLevel();
        const double grams = std::max(
            5.0,
            DietCalculator::calculateDailyGrams(*pet, manager->getMedicalRecord()) * 0.35
        );

        const bool fed = bowlRegistry.dispense(
            pet->getId(), grams,
            manager->getMonitor(),
            manager->getLogger()
        );
        if (!fed) {
            continue;
        }

        pet->setHungerLevel(std::min(pet->getHungerLevel(), 10));
        ++stats.feedingOccurrences;

        FeedingEvent feedingEvent(
            pet->getId(),
            pet->getName(),
            grams,
            previousHunger,
            pet->getHungerLevel()
        );
        logEvent(feedingEvent.toString());
    }
}

void SimulationEngine::performPeriodicVetCheck() {
    for (Pet* pet : manager->getAllPets()) {
        if (!pet) {
            continue;
        }

        ++stats.vetChecksPerformed;
        logEvent(vetInspector->performHealthCheck(*pet, manager->getMedicalRecord()));
    }
}

void SimulationEngine::displayStatus() const {
    std::cout << getShelterStatus() << '\n';
    const std::size_t historyCount = std::min<std::size_t>(lastEvents.size(), 5);
    for (std::size_t index = lastEvents.size() - historyCount; index < lastEvents.size(); ++index) {
        std::cout << "  " << lastEvents[index] << '\n';
    }
}

void SimulationEngine::logEvent(const std::string& message) {
    const std::string formattedMessage = "[Tick " + std::to_string(currentTick) + "] " + message;
    eventLog.push_back(formattedMessage);
    lastEvents.push_back(formattedMessage);
    manager->getLogger().info("SIM", formattedMessage);
}

void SimulationEngine::updateStatisticsForEvent(const SimulationEvent& event) {
    switch (event.getType()) {
        case EventType::ANIMAL_ARRIVED:
            ++stats.animalsArrived;
            break;
        case EventType::ANIMAL_ADOPTED:
            ++stats.animalsAdopted;
            break;
        case EventType::VACCINATION:
            ++stats.vaccinationsGiven;
            break;
        case EventType::VET_CHECK:
            ++stats.vetChecksPerformed;
            break;
        default:
            break;
    }
}

void SimulationEngine::capturePopulationSnapshot() {
    const auto animals = manager->getAllPets();
    if (animals.empty()) {
        return;
    }

    double totalHunger = 0.0;
    double totalHealth = 0.0;
    for (const Pet* pet : animals) {
        totalHunger += pet->getHungerLevel();
        totalHealth += pet->getHealthLevel();
    }

    stats.accumulatedAverageHunger += totalHunger / static_cast<double>(animals.size());
    stats.accumulatedAverageHealth += totalHealth / static_cast<double>(animals.size());
    ++stats.samples;
}

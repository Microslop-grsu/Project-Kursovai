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
        if (currentTick % statusDisplayInterval == 0) {
            displayStatus();
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

std::string SimulationEngine::getShelterStatus() const {
    const auto animals = manager->getAllPets();
    double totalHunger = 0.0;
    double totalHealth = 0.0;

    for (const Pet* pet : animals) {
        totalHunger += pet->getHungerLevel();
        totalHealth += pet->getHealthLevel();
    }

    const double averageHunger = animals.empty() ? 0.0 : totalHunger / static_cast<double>(animals.size());
    const double averageHealth = animals.empty() ? 0.0 : totalHealth / static_cast<double>(animals.size());

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(1);
    stream << "Tick " << currentTick
           << " | pets: " << animals.size()
           << " | avg hunger: " << averageHunger
           << " | avg health: " << averageHealth;
    return stream.str();
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
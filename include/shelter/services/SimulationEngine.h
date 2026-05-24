#pragma once
#include "EventGenerator.h"
#include "EventProcessor.h"
#include "VetInspector.h"
#include "SmartBowl.h"
#include <memory>
#include <string>
#include <vector>

class ShelterManager;

class SimulationEngine {
public:
    SimulationEngine(
        std::shared_ptr<ShelterManager> manager,
        const SimulationConfig& config = {}
    );

    void run(int totalTicks, int displayInterval = 1);
    void tick();

    int getCurrentTick() const;
    std::vector<std::string> getLastEvents() const;
    std::string getShelterStatus() const;

    void saveStatistics(const std::string& filename) const;

private:
    std::shared_ptr<ShelterManager> manager;
    std::shared_ptr<EventGenerator> eventGenerator;
    std::shared_ptr<VetInspector> vetInspector;
    std::shared_ptr<EventProcessor> eventProcessor;
    SimulationConfig config;

    SmartBowlRegistry bowlRegistry;
    int ticksPerDay = 10;

    int currentTick = 0;
    int statusDisplayInterval = 1;
    std::vector<std::string> eventLog;
    std::vector<std::string> lastEvents;

    struct Statistics {
        int animalsArrived = 0;
        int animalsAdopted = 0;
        int vaccinationsGiven = 0;
        int vetChecksPerformed = 0;
        int feedingOccurrences = 0;
        int criticalHealthCases = 0;
        double accumulatedAverageHunger = 0.0;
        double accumulatedAverageHealth = 0.0;
        int samples = 0;
    } stats;

    void updateAnimalStates();
    void performHungerCheck();
    void performPeriodicVetCheck();
    void displayStatus() const;
    void logEvent(const std::string& message);
    void updateStatisticsForEvent(const SimulationEvent& event);
    void capturePopulationSnapshot();
};
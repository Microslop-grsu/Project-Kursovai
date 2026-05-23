#pragma once
#include "SimulationEvent.h"
#include <memory>
#include <random>
#include <vector>

class Pet;

struct SimulationConfig {
    float arrivalProbability = 0.10f;
    float adoptionProbability = 0.07f;
    float vaccinationProbability = 0.05f;
    float vetCheckProbability = 0.03f;
    int hungerIncreasePerTick = 12;
    int hungerFeedingThreshold = 50;
    int criticalHungerThreshold = 80;
    int healthPenaltyPerCriticalTick = 5;
    int periodicVetInterval = 30;
};

class EventGenerator {
public:
    explicit EventGenerator(const SimulationConfig& config = {});

    std::vector<std::shared_ptr<SimulationEvent>> generateEvents(
        int currentTick,
        const std::vector<Pet*>& animals
    );

    void setConfig(const SimulationConfig& config);
    SimulationConfig getConfig() const;

private:
    SimulationConfig config;
    std::mt19937 randomEngine;

    bool shouldEventOccur(float probability);
    std::shared_ptr<SimulationEvent> createAnimalArrival();
    std::shared_ptr<SimulationEvent> createAnimalAdoption(Pet& pet);
    std::shared_ptr<SimulationEvent> createVaccination(Pet& pet);
    std::shared_ptr<SimulationEvent> createVetCheck(Pet& pet);

    ArrivingPetData createArrivalData();
    std::string createDateString(std::time_t timestamp) const;
};

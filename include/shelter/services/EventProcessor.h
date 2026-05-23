#pragma once
#include "SimulationEvent.h"
#include "VetInspector.h"
#include <memory>
#include <string>

class ShelterManager;

class EventProcessor {
public:
    EventProcessor(
        std::shared_ptr<ShelterManager> manager,
        std::shared_ptr<VetInspector> vetInspector
    );

    bool processEvent(const std::shared_ptr<SimulationEvent>& event);
    std::string getLastError() const;

private:
    std::shared_ptr<ShelterManager> manager;
    std::shared_ptr<VetInspector> vetInspector;
    std::string lastError;

    bool handleAnimalArrived(const std::shared_ptr<SimulationEvent>& event);
    bool handleAnimalAdopted(const std::shared_ptr<SimulationEvent>& event);
    bool handleVaccination(const std::shared_ptr<SimulationEvent>& event);
    bool handleVetCheck(const std::shared_ptr<SimulationEvent>& event);
};

#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../storage/PetRepository.h"
#include "../core/MedicalRecord.h"
#include "FeedingMonitor.h"
#include "../utils/Logger.h"

class ShelterManager {
public:
    ShelterManager(const std::string& logPath);

    bool loadData(const std::string& path);
    bool saveData(const std::string& path);

    const PetRepository& getRepo() const { return repo; }
    const MedicalRecord& getMedicalRecord() const { return medicalRecord; }
    FeedingMonitor& getMonitor() { return monitor; }
    Logger& getLogger() { return logger; }

    void addVaccination(short petId, const std::string& vaccine, const std::string& date);
    void checkHungryPets();

private:
    Logger logger;
    PetRepository repo;
    MedicalRecord medicalRecord;
    FeedingMonitor monitor;
};



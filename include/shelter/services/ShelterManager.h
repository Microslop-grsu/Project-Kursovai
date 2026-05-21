#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../storage/PetRepository.h"
#include "../core/MedicalRecord.h"
#include "FeedingMonitor.h"
#include "DietCalculator.h"
#include "../utils/Logger.h"

class ShelterManager {
public:
    ShelterManager(const std::string& logPath);

    bool loadData(const std::string& path);
    bool saveData(const std::string& path);

    const PetRepository& getRepo() const { return repo; }
    const MedicalRecord& getMedicalRecord() const { return medicalRecord; }
    MedicalRecord& getMedicalRecord() { return medicalRecord; }   // ← не-const версия
    FeedingMonitor& getMonitor() { return monitor; }
    Logger& getLogger() { return logger; }

    void addVaccination(short petId, const std::string& vaccine, const std::string& date);
    void addHealthIssue(const std::string& issue);  // ← новый: передаёт в MedicalRecord
    void checkHungryPets();
    void feedPet(const std::string& petName, double grams);
    void printDietInfo(short petId) const;
    void checkOverfeeding();                

private:
    Logger logger;
    PetRepository repo;
    MedicalRecord medicalRecord;
    FeedingMonitor monitor;
};
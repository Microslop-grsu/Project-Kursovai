#pragma once
#include <memory>
#include <string>
#include <vector>
#include "../core/MedicalRecord.h"
#include "../storage/PetRepository.h"
#include "../utils/Logger.h"
#include "DietCalculator.h"
#include "FeedingMonitor.h"

class ShelterManager {
public:
    explicit ShelterManager(const std::string& logPath);

    bool loadData(const std::string& path);
    bool saveData(const std::string& path);
    bool addPet(std::unique_ptr<Pet> pet);
    bool removePet(short id);

    const PetRepository& getRepo() const { return repo; }
    const MedicalRecord& getMedicalRecord() const { return medicalRecord; }
    MedicalRecord& getMedicalRecord() { return medicalRecord; }
    FeedingMonitor& getMonitor() { return monitor; }
    Logger& getLogger() { return logger; }
    Pet* getPetById(short id) const;
    std::vector<Pet*> getAllPets() const;
    size_t getPetCount() const;
    short generateNextPetId() const;

    void addVaccination(short petId, const std::string& vaccine, const std::string& date);
    void addHealthIssue(const std::string& issue);
    void checkHungryPets();
    void feedPet(const std::string& petName, double grams);
    void feedPet(short petId, double grams);
    void printDietInfo(short petId) const;
    void checkOverfeeding();

private:
    Logger logger;
    PetRepository repo;
    MedicalRecord medicalRecord;
    FeedingMonitor monitor;
};

#include "../../include/shelter/services/ShelterManager.h"
#include "../../include/shelter/storage/DataLoader.h"

ShelterManager::ShelterManager(const std::string& logPath) : logger(logPath) {}

bool ShelterManager::loadData(const std::string& path) {
    return DataLoader::loadFromFile(path, repo);
}

bool ShelterManager::saveData(const std::string& path) {
    DataLoader loader;
    return loader.saveToFile(path, repo);
}

void ShelterManager::addVaccination(short petId, const std::string& vaccine, const std::string& date) {
    medicalRecord.addVaccination(petId, vaccine, date);
    logger.info("MEDICAL", "Added vaccination for pet ID: " + std::to_string(petId));
}

void ShelterManager::checkHungryPets() {
    std::vector<Pet*> petPtrs;
    for (const auto& pet : repo.getAll()) {
        petPtrs.push_back(pet.get());
    }
    monitor.checkStarvation(logger, petPtrs);
}



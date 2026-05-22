#include "../../include/shelter/services/ShelterManager.h"
#include "../../include/shelter/storage/DataLoader.h"
#include "../../include/shelter/services/DietCalculator.h"
#include <iostream>

ShelterManager::ShelterManager(const std::string& logPath) : logger(logPath) {}

bool ShelterManager::loadData(const std::string& path) {
    return DataLoader::loadFromFile(path, repo);
}
bool ShelterManager::saveData(const std::string& path) {
    DataLoader loader;
    return loader.saveToFile(path, repo);
}

void ShelterManager::addVaccination(short petId, const std::string& vaccine,
                                     const std::string& date) {
    medicalRecord.addVaccination(petId, vaccine, date);
    logger.info("MEDICAL", "Added vaccination for pet ID: " + std::to_string(petId));
}

void ShelterManager::addHealthIssue(const std::string& issue) {
    medicalRecord.addHealthIssue(issue);
    logger.info("DIET", "Health issue registered: " + issue);
}

void ShelterManager::checkHungryPets() {
    std::vector<Pet*> ptrs;
    for (const auto& pet : repo.getAll()) ptrs.push_back(pet.get());
    monitor.checkStarvation(logger, ptrs);
}

void ShelterManager::feedPet(const std::string& petName, double grams) {
    monitor.feed(petName, grams);
    logger.info("FEEDING", "Fed " + petName + " with "
                + std::to_string(static_cast<int>(grams)) + " g");
}

void ShelterManager::printDietInfo(short petId) const {
    Pet* pet = repo.findByID(petId);
    if (!pet) { std::cout << "Питомец с ID " << petId << " не найден.\n"; return; }
    std::cout << DietCalculator::getDietSummary(*pet, medicalRecord);
}

void ShelterManager::checkOverfeeding() {
    std::vector<Pet*> ptrs;
    for (const auto& pet : repo.getAll()) ptrs.push_back(pet.get());
    monitor.checkOverfeeding(logger, ptrs, medicalRecord);
}
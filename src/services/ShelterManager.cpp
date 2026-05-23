#include "../../include/shelter/services/ShelterManager.h"
#include "../../include/shelter/services/DietCalculator.h"
#include "../../include/shelter/storage/DataLoader.h"
#include <algorithm>
#include <iostream>

ShelterManager::ShelterManager(const std::string& logPath) : logger(logPath) {}

bool ShelterManager::loadData(const std::string& path) {
    return DataLoader::loadFromFile(path, repo);
}

bool ShelterManager::saveData(const std::string& path) {
    DataLoader loader;
    return loader.saveToFile(path, repo);
}

bool ShelterManager::addPet(std::unique_ptr<Pet> pet) {
    if (!pet) {
        return false;
    }

    if (repo.findByID(pet->getId()) != nullptr) {
        logger.warning("SHELTER", "Attempted to add a duplicate pet ID: " + std::to_string(pet->getId()));
        return false;
    }

    logger.info(
        "SHELTER",
        "Added pet #" + std::to_string(pet->getId()) + " (" + pet->getType() + " " + pet->getName() + ")"
    );
    repo.add(std::move(pet));
    return true;
}

bool ShelterManager::removePet(short id) {
    Pet* pet = repo.findByID(id);
    if (!pet) {
        logger.warning("SHELTER", "Attempted to remove a missing pet ID: " + std::to_string(id));
        return false;
    }

    const std::string petName = pet->getName();
    const bool removed = repo.remove(id);
    if (removed) {
        logger.info("SHELTER", "Removed pet #" + std::to_string(id) + " (" + petName + ")");
    }
    return removed;
}

Pet* ShelterManager::getPetById(short id) const {
    return repo.findByID(id);
}

std::vector<Pet*> ShelterManager::getAllPets() const {
    return repo.getAllRaw();
}

size_t ShelterManager::getPetCount() const {
    return repo.size();
}

short ShelterManager::generateNextPetId() const {
    return static_cast<short>(repo.getMaxId() + 1);
}

void ShelterManager::addVaccination(short petId, const std::string& vaccine, const std::string& date) {
    medicalRecord.addVaccination(petId, vaccine, date);
    logger.info("MEDICAL", "Added vaccination for pet ID: " + std::to_string(petId));
}

void ShelterManager::addHealthIssue(const std::string& issue) {
    medicalRecord.addHealthIssue(issue);
    logger.info("DIET", "Health issue registered: " + issue);
}

void ShelterManager::checkHungryPets() {
    monitor.checkStarvation(logger, getAllPets());
}

void ShelterManager::feedPet(const std::string& petName, double grams) {
    short firstMatchId = -1;
    int matches = 0;
    for (Pet* pet : getAllPets()) {
        if (pet && pet->getName() == petName) {
            if (firstMatchId < 0) {
                firstMatchId = pet->getId();
            }
            ++matches;
        }
    }

    if (firstMatchId < 0) {
        logger.warning("FEEDING", "Attempted to feed a missing pet named " + petName);
        return;
    }

    if (matches > 1) {
        logger.warning(
            "FEEDING",
            "Multiple pets named " + petName + " were found. Feeding the first match by ID."
        );
    }

    feedPet(firstMatchId, grams);
}

void ShelterManager::feedPet(short petId, double grams) {
    Pet* pet = repo.findByID(petId);
    if (!pet) {
        logger.warning("FEEDING", "Attempted to feed a missing pet ID: " + std::to_string(petId));
        return;
    }

    monitor.feed(petId, grams);
    pet->setHungerLevel(std::min(pet->getHungerLevel(), 10));
    logger.info(
        "FEEDING",
        "Fed " + pet->getName() + " (#" + std::to_string(petId) + ") with "
        + std::to_string(static_cast<int>(grams)) + " g"
    );
}

void ShelterManager::printDietInfo(short petId) const {
    Pet* pet = repo.findByID(petId);
    if (!pet) {
        std::cout << "Pet with ID " << petId << " was not found.\n";
        return;
    }
    std::cout << DietCalculator::getDietSummary(*pet, medicalRecord);
}

void ShelterManager::checkOverfeeding() {
    monitor.checkOverfeeding(logger, getAllPets(), medicalRecord);
}

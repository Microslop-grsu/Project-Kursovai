#include "../../include/shelter/services/ShelterManager.h"
#include "../../include/shelter/services/DietCalculator.h"
#include "../../include/shelter/storage/DataLoader.h"
#include "../../include/shelter/storage/PetFactory.h"
#include <algorithm>
#include <iostream>

ShelterManager::ShelterManager(const std::string& logPath, std::string dataPath, const std::string& storageLogPath)
    : logger(logPath), dataLoader(storageLogPath), dataPath(std::move(dataPath)) {}

bool ShelterManager::loadData(const std::string& path) {
    const bool loaded = dataLoader.loadFromFile(path, repo);
    if (loaded) {
        dataPath = path;
    }
    return loaded;
}

bool ShelterManager::saveData(const std::string& path) {
    const bool saved = dataLoader.saveToFile(path, repo);
    if (saved) {
        dataPath = path;
    }
    return saved;
}

bool ShelterManager::addPet(std::unique_ptr<Pet> pet) {
    if (!pet) {
        return false;
    }

    const short petId = pet->getId();
    const std::string petType = pet->getType();
    const std::string petName = pet->getName();

    if (repo.findByID(pet->getId()) != nullptr) {
        logger.warning("SHELTER", "Attempted to add a duplicate pet ID: " + std::to_string(petId));
        return false;
    }

    repo.add(std::move(pet));
    logger.info("SHELTER", "Added pet #" + std::to_string(petId) + " (" + petType + " " + petName + ")");

    if (!dataPath.empty() && !saveData(dataPath)) {
        repo.remove(petId);
        logger.error("SHELTER", "Failed to save pets data after adding pet #" + std::to_string(petId));
        return false;
    }

    return true;
}

bool ShelterManager::removePet(short id) {
    Pet* pet = repo.findByID(id);
    if (!pet) {
        logger.warning("SHELTER", "Attempted to remove a missing pet ID: " + std::to_string(id));
        return false;
    }

    const std::string petName = pet->getName();
    const nlohmann::json removedPetSnapshot = pet->toJson();
    const bool removed = repo.remove(id);
    if (removed) {
        logger.info("SHELTER", "Removed pet #" + std::to_string(id) + " (" + petName + ")");

        if (!dataPath.empty() && !saveData(dataPath)) {
            auto restoredPet = PetFactory::createFromJson(removedPetSnapshot);
            if (restoredPet) {
                repo.add(std::move(restoredPet));
            }
            logger.error("SHELTER", "Failed to save pets data after removing pet #" + std::to_string(id));
            return false;
        }
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

void ShelterManager::addHealthIssue(short petId, const std::string& issue) {
    medicalRecord.addHealthIssue(petId, issue);
    Pet* pet = repo.findByID(petId);
    std::string petLabel = pet ? (pet->getName() + " (#" + std::to_string(petId) + ")") : std::to_string(petId);
    logger.info("DIET", "Health issue registered for " + petLabel + ": " + issue);
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

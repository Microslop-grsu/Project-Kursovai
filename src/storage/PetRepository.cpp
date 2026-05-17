#include "../../include/shelter/storage/PetRepository.h"
#include <algorithm>

void PetRepository::add(std::unique_ptr<Pet> pet) {
    if (pet) {
        pets.push_back(std::move(pet));
    }
}

void PetRepository::addRaw(Pet* pet) {
    if (pet) {
        pets.push_back(std::unique_ptr<Pet>(pet));
    }
}

Pet* PetRepository::findByID(short id) const {
    auto it = std::find_if(pets.begin(), pets.end(),
            [id](const std::unique_ptr<Pet>& pet) {
                return pet->getId() == id;
            });

    if (it != pets.end()) {
        return it->get();
    }
    return nullptr;
}

bool PetRepository::remove(short id) {
    auto it = std::find_if(pets.begin(), pets.end(),
            [id](const std::unique_ptr<Pet>& pet) {
                return pet->getId() == id;
            });

    if (it != pets.end()) {
        pets.erase(it);
        return true;
    }
    return false;
}

const std::vector<std::unique_ptr<Pet>>& PetRepository::getAll() const {return pets;}

std::vector<Pet*> PetRepository::findByCriteria(const SearchCriteria& crit) const {
    std::vector<Pet*> result;
    for (const auto& pet : pets) {
        bool matches = true;

        if (!crit.name.empty() && pet->getName() != crit.name)
            matches = false;
        else if (!crit.type.empty() && pet->getType() != crit.type)
            matches = false;
        else if (crit.min_age > 0 && pet->getAge() < crit.min_age)
            matches = false;
        else if (crit.max_age > 0 && pet->getAge() > crit.max_age)
            matches = false;
        else if (crit.min_weight > 0 && pet->getWeight() < crit.min_weight)
            matches = false;
        else if (crit.max_weight > 0 && pet->getWeight() > crit.max_weight)
            matches = false;

        if (matches) {
            result.push_back(pet.get());
        }
    }
    return result;
}

void PetRepository::clear() {
    pets.clear();
}

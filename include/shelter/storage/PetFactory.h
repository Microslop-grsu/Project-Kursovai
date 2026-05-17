#ifndef SHELTERCORE_PETFACTORY_H
#define SHELTERCORE_PETFACTORY_H
#include <memory>
#include <string>
#include "../core/Pet.h"

class PetFactory {
public:
    static std::unique_ptr<Pet> createFromJson(const nlohmann::json& jsonData);
};

#endif //SHELTERCORE_PETFACTORY_H

#include "../../include/shelter/utils/PetFactory.h"
#include "../../include/shelter/utils/json.hpp"
#include "../../include/shelter/core/Dog.h"
#include "../../include/shelter/core/Cat.h"
#include "../../include/shelter/core/Bird.h"
#include "../../include/shelter/core/Exotic.h"

std::unique_ptr<Pet> PetFactory::createFromJson(const nlohmann::json& jsonData) {
    short id = jsonData["id"];
    std::string type = jsonData["type"];
    std::string name = jsonData["name"];
    int age = jsonData["age"];
    double weight = jsonData["weight"];

    if (type == "Dog") {
        bool hasCommands = jsonData.value("hasCommands", false);
        return std::make_unique<Dog>(id, name, age, weight, hasCommands);
    }
    else if (type == "Cat") {
        std::string breed = jsonData.value("furType", "Unknown");
        return std::make_unique<Cat>(id, name, age, weight, breed);
    }
    else if (type == "Bird") {
        double wingSpan = jsonData["wingSpan"];
        return std::make_unique<Bird>(id, name, age, weight, wingSpan);
    }
    else if (type == "Exotic") {
        double temperature = jsonData["temperature"];
        return std::make_unique<Exotic>(id, name, age, weight, temperature);
    }

    return nullptr;
}

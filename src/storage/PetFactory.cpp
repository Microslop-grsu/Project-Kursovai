#include "../../include/shelter/storage/PetFactory.h"
#include "../../include/shelter/core/Bird.h"
#include "../../include/shelter/core/Cat.h"
#include "../../include/shelter/core/Dog.h"
#include "../../include/shelter/core/Exotic.h"
#include "../../include/shelter/storage/json.hpp"

namespace {
ActivityLevel parseActivityLevel(const nlohmann::json& jsonData) {
    const std::string value = jsonData.value("activityLevel", "NORMAL");
    if (value == "LOW") {
        return ActivityLevel::LOW;
    }
    if (value == "HIGH") {
        return ActivityLevel::HIGH;
    }
    return ActivityLevel::NORMAL;
}
}

std::unique_ptr<Pet> PetFactory::createFromJson(const nlohmann::json& jsonData) {
    const short id = jsonData["id"];
    const std::string type = jsonData["type"];
    const std::string name = jsonData["name"];
    const int age = jsonData["age"];
    const double weight = jsonData["weight"];

    std::unique_ptr<Pet> pet;
    if (type == "Dog") {
        const bool hasCommands = jsonData.value("hasCommands", false);
        pet = std::make_unique<Dog>(id, name, age, weight, hasCommands);
    } else if (type == "Cat") {
        const std::string breed = jsonData.value("furType", "Unknown");
        pet = std::make_unique<Cat>(id, name, age, weight, breed);
    } else if (type == "Bird") {
        const double wingSpan = jsonData["wingSpan"];
        pet = std::make_unique<Bird>(id, name, age, weight, wingSpan);
    } else if (type == "Exotic") {
        const double temperature = jsonData["temperature"];
        const double humidity = jsonData.value("humidity", 50.0);
        pet = std::make_unique<Exotic>(id, name, age, weight, temperature, humidity);
    }

    if (!pet) {
        return nullptr;
    }

    pet->setActivityLevel(parseActivityLevel(jsonData));
    pet->setHungerLevel(jsonData.value("hungerLevel", jsonData.value("isHungry", false) ? 51 : 0));
    pet->setHealthLevel(jsonData.value("healthLevel", 100));
    return pet;
}

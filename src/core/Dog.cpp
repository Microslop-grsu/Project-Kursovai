#include "../../include/shelter/core/Dog.h"
#include <iostream>

Dog::Dog(short id, const std::string& name, int age, double weight, bool knowsCommands)
    : Pet(id,name, age, weight), knowsCommands(knowsCommands) {
}

Dog::~Dog() {}

nlohmann::json Dog::toJson() const {
    nlohmann::json j = Pet::toJson();
    j["hasCommands"] = knowsCommands;
    return j;
}

std::string Dog::makeSound() const {
    return "Woof!";
}

std::string Dog::getDiet() const {
    return "Dry kibble or raw meat, 2 times a day";
}

std::string Dog::getCareInstructions() const {
    if (knowsCommands) {
        return "Daily walks required. Knows commands keep up training sessions.";
    }
    return "Daily walks required. Needs basic obedience training.";
}

bool Dog::getKnowsCommands() const {
    return knowsCommands;
}

void Dog::printInfo() const {
    std::cout
        << id << " "
        << "[Dog] ";
    Pet::printInfo();
}

void Dog::printDetailInfo() const {
    Pet::printDetailInfo();
    std::cout
    << "Знает команды: " << (knowsCommands ? "да" : "нет") << std::endl;
}

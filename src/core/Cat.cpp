#include "../../include/shelter/core/Cat.h"
#include <iostream>

Cat::Cat(short id, const std::string& name, int age, double weight, const std::string& furType)
    : Pet(id, name, age, weight), furType(furType) {
}

Cat::~Cat() {}

std::string Cat::makeSound() const {
    return "Meow!";
}

std::string Cat::getDiet() const {
    return "Wet or dry cat food, 2-3 times a day";
}

std::string Cat::getCareInstructions() const {
    if (furType == "long") {
        return "Daily brushing required. Regular vet checkups.";
    }
    else if (furType == "hairless") {
        return "Weekly bath required. Keep warm, sensitive to cold.";
    }
    return "Brush once a week. Standard vet checkups.";
}

nlohmann::json Cat::toJson() const {
    nlohmann::json j = Pet::toJson();
    j["fyrType"] = furType;
    return j.dump();
}

std::string Cat::getFurType() const {
    return furType;
}

void Cat::printInfo() const {
    std::cout
        << id << " "
        << "[Cat] ";
    Pet::printInfo();
}

void Cat::printDetailInfo() const {
    Pet::printDetailInfo();
    std::cout
    << "Тип шерсти: " << furType << std::endl;
}

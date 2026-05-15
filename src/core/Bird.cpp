#include "../../include/shelter/core/Bird.h"
#include <iostream>

Bird::Bird(short id, const std::string& name, int age, double weight, double wingSpan)
    : Pet(id, name, age, weight), wingSpan(wingSpan) {
}

Bird::~Bird() {}

std::string Bird::makeSound() const {
    return "Tweet!";
}

std::string Bird::getDiet() const {
    return "Seeds, fruits and vegetables, fresh water daily";
}

std::string Bird::getCareInstructions() const {
    if (wingSpan > 50.0) {
        return "Large cage required (wingspan " + std::to_string(wingSpan) + "cm). Daily flight time outside cage.";
    }
    return "Standard cage suitable (wingspan " + std::to_string(wingSpan) + "cm). Social interaction important.";
}

nlohmann::json Bird::toJson() const {
    nlohmann::json j = Pet::toJson();
    j["wingSpan"] = wingSpan;
    return j.dump();
}

double Bird::getWingSpan() const {
    return wingSpan;
}

void Bird::printInfo() const {
    std::cout
        << id << " "
        << "[Bird] ";
    Pet::printInfo();
}

void Bird::printDetailInfo() const {
    Pet::printDetailInfo();
    std::cout
    << "Размах крыльев: " << wingSpan << "см" << std::endl;
}

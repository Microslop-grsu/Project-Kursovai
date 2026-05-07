#include "Bird.h"
#include <iostream>

Bird::Bird(const std::string& name, int age, double weight, double wingSpan)
    : Pet(name, age, weight), wingSpan(wingSpan) {
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

double Bird::getWingSpan() const {
    return wingSpan;
}

void Bird::printInfo() const {
    Pet::printInfo();
    std::cout << "[Bird] wingSpan: " << wingSpan << "cm" << std::endl;
}
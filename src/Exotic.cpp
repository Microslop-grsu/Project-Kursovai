#include "Exotic.h"
#include <iostream>
#include <sstream>

Exotic::Exotic(const std::string& name, int age, double weight, double requiredTemperature)
    : Pet(name, age, weight), requiredTemperature(requiredTemperature) {
}

Exotic::~Exotic() {}

std::string Exotic::makeSound() const {
    return "exotic sound";
}

std::string Exotic::getDiet() const {
    return "exotic diet";
}

std::string Exotic::getCareInstructions() const {
    std::stringstream ss;
    ss << "temperature " << requiredTemperature << "C. ";
    return ss.str();
}

double Exotic::getRequiredTemperature() const {
    return requiredTemperature;
}

void Exotic::printInfo() const {
    std::cout << "[Exotic] " << name
        << " | age: " << age
        << " | weight: " << weight << "kg"
        << " | hungry: " << (isHungry ? "YES" : "no")
        << " | temp: " << requiredTemperature << "C"
        << std::endl;
}
void Exotic::setRequiredTemperature(double temp) {
    requiredTemperature = temp;
}

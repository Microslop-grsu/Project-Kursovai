#include "../../include/shelter/core/Pet.h"
#include <iostream>

Pet::Pet(const std::string& name, int age, double weight)
    : name(name), age(age), weight(weight), isHungry(false) {
}

Pet::~Pet() {}

std::string Pet::getName()     const { return name; }
int         Pet::getAge()      const { return age; }
double      Pet::getWeight()   const { return weight; }
bool        Pet::getIsHungry() const { return isHungry; }

void Pet::setIsHungry(bool hungry) {
    isHungry = hungry;
}

void Pet::printInfo() const {
    std::cout << name
        << " | age: " << age
        << " | hungry: " << (isHungry ? "YES" : "no")
        << std::endl;
}

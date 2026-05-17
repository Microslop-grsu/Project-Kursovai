#include "../../include/shelter/core/Pet.h"
#include <iostream>

Pet::Pet(short id, const std::string& name, int age, double weight)
    : id(id), name(name), age(age), weight(weight), isHungry(false) {
}

Pet::~Pet() {}

short Pet::getId() const {return id;}
std::string Pet::getName()     const { return name; }
int         Pet::getAge()      const { return age; }
double      Pet::getWeight()   const { return weight; }
bool        Pet::getIsHungry() const { return isHungry; }

void Pet::setIsHungry(bool hungry) {
    isHungry = hungry;
}

nlohmann::json Pet::toJson() const {
    nlohmann::json j;
    j["id"] = id;
    j["type"] = getType();
    j["name"] = name;
    j["age"] = age;
    j["weight"] = weight;
    return j;
}

void Pet::printInfo() const {
    std::cout
        << name
        << " | age: " << age
        << " | hungry: " << (isHungry ? "YES" : "no")
        << std::endl;
}
void Pet::printDetailInfo() const {
    std::cout
        << "Id - " << id << std::endl
        << "Тип    : " << getType() << std::endl
        << "Кличка : " << name << std::endl
        << "Возраст: " << age << std::endl
        << "Вес    : " << weight << "кг"<< std::endl;
}

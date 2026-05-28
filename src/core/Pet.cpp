#include "../../include/shelter/core/Pet.h"
#include <algorithm>
#include <iostream>

Pet::Pet(short id, const std::string& name, int age, double weight, ActivityLevel activity)
    : id(id),
      name(name),
      age(age),
      weight(weight),
      activityLevel(activity),
      hungerLevel(0),
      healthLevel(100),
      consecutiveLowHungerTicks(0) {}

Pet::~Pet() {}

short Pet::getId() const { return id; }
std::string Pet::getName() const { return name; }
int Pet::getAge() const { return age; }
double Pet::getWeight() const { return weight; }
ActivityLevel Pet::getActivityLevel() const { return activityLevel; }
int Pet::getHungerLevel() const { return hungerLevel; }
int Pet::getHealthLevel() const { return healthLevel; }
int Pet::getConsecutiveLowHungerTicks() const { return consecutiveLowHungerTicks; }


void Pet::setActivityLevel(ActivityLevel level) {
    activityLevel = level;
}

void Pet::setHungerLevel(int hunger) {
    hungerLevel = std::clamp(hunger, 0, 100);
}

void Pet::setHealthLevel(int health) {
    healthLevel = std::clamp(health, 0, 100);
}

void Pet::increaseHunger(int amount) {
    setHungerLevel(hungerLevel + amount);
}

void Pet::decreaseHunger(int amount) {
    setHungerLevel(hungerLevel - amount);
}

void Pet::changeHealth(int delta) {
    setHealthLevel(healthLevel + delta);
}

void Pet::resetConsecutiveLowHungerTicks() {
    consecutiveLowHungerTicks = 0;
}

void Pet::increaseConsecutiveLowHungerTicks() {
    ++consecutiveLowHungerTicks;
}

std::string Pet::activityToString(ActivityLevel level) {
    switch (level) {
        case ActivityLevel::LOW:
            return "LOW";
        case ActivityLevel::HIGH:
            return "HIGH";
        default:
            return "NORMAL";
    }
}

nlohmann::json Pet::toJson() const {
    nlohmann::json j;
    j["id"] = id;
    j["type"] = getType();
    j["name"] = name;
    j["age"] = age;
    j["weight"] = weight;
    j["activityLevel"] = activityToString(activityLevel);
    j["hungerLevel"] = hungerLevel;
    j["healthLevel"] = healthLevel;
    return j;
}

void Pet::printInfo() const {
    std::cout
        << name
        << " | age: " << age
        << " | hunger: " << hungerLevel
        << " | health: " << healthLevel
        << " | activity: " << activityToString(activityLevel)
        << std::endl;
}

void Pet::printDetailInfo() const {
    std::cout
        << "Id - " << id << std::endl
        << "Type     : " << getType() << std::endl
        << "Name     : " << name << std::endl
        << "Age      : " << age << std::endl
        << "Weight   : " << weight << " kg" << std::endl
        << "Health   : " << healthLevel << std::endl
        << "Hunger   : " << hungerLevel << std::endl
        << "Activity : " << activityToString(activityLevel) << std::endl;
}

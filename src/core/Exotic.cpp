#include "../../include/shelter/core/Exotic.h"
#include "../../include/shelter/utils/Logger.h"
#include <iostream>
#include <sstream>

Exotic::Exotic(short id, const std::string& name, int age, double weight, double requiredTemperature, double humidity)
    : Pet(id, name, age, weight), requiredTemperature(requiredTemperature), humidity(humidity) {}

Exotic::~Exotic() {}

std::string Exotic::makeSound() const {
    return "exotic sound";
}

std::string Exotic::getDiet() const {
    return "exotic diet";
}

std::string Exotic::getCareInstructions() const {
    std::stringstream ss;
    ss << "temperature " << requiredTemperature << "C, humidity "
       << MIN_HUMIDITY << "-" << MAX_HUMIDITY << "%.";
    return ss.str();
}

nlohmann::json Exotic::toJson() const {
    nlohmann::json j = Pet::toJson();
    j["temperature"] = requiredTemperature;
    j["humidity"] = humidity;
    return j;
}

double Exotic::getRequiredTemperature() const {
    return requiredTemperature;
}

void Exotic::setRequiredTemperature(double temp)
{
    requiredTemperature = temp;
}

double Exotic::getHumidity() const
{
    return humidity;
}

void Exotic::setHumidity(double hum)
{
    humidity = hum;
}

void Exotic::checkHumidity(Logger &logger) const
{
    if (humidity < MIN_HUMIDITY || humidity > MAX_HUMIDITY)
    {
        std::string msg = "Exotic animal " + name + " has critical humidity: " + std::to_string(humidity) + "% (normal range: " + std::to_string(MIN_HUMIDITY) + "-" + std::to_string(MAX_HUMIDITY) + "%)";
        logger.warning("HUMIDITY", msg);
    }
}


void Exotic::printInfo() const {
    std::cout
        << id << " "
        << "[Exotic] ";
    Pet::printInfo();

}

void Exotic::printDetailInfo() const {
    Pet::printDetailInfo();
    std::cout
    << "Условия климата: " << requiredTemperature << "C" << std::endl
    << "Влажность       : " << humidity << "%" << std::endl;
}

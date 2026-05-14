#pragma once
#include "Pet.h"
#include <string>

class Logger;

class Exotic : public Pet {
public:

    Exotic(short id, const std::string &name, int age, double weight, double requiredTemperature, double humidity = 50.0);

    virtual ~Exotic();

    std::string getType() const override { return "Bird"; }
    nlohmann::json toJson() const override;
    std::string makeSound() const override;
    std::string getDiet() const override;
    std::string getCareInstructions() const override;

    double getRequiredTemperature() const;
    void setRequiredTemperature(double temp);double getHumidity() const;
    void setHumidity(double hum);
    void checkHumidity(Logger &logger) const; // генерация Warning
    void printInfo() const override;

private:
    double requiredTemperature;
    double humidity;

    static constexpr double MIN_HUMIDITY = 40.0;
    static constexpr double MAX_HUMIDITY = 70.0;
};

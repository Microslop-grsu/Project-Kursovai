#pragma once
#include "Pet.h"
#include <string>

class Exotic : public Pet {
public:

    Exotic(const std::string& name, int age, double weight, double requiredTemperature);
    
    virtual ~Exotic();

    std::string makeSound() const override;
    std::string getDiet() const override;
    std::string getCareInstructions() const override;
    
    double getRequiredTemperature() const;
    void setRequiredTemperature(double temp); 
    void printInfo() const override;
    
private:
    double requiredTemperature;
};
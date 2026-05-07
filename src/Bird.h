#pragma once
#include "Pet.h"

class Bird : public Pet {
public:
    Bird(const std::string& name, int age, double weight, double wingSpan);
    virtual ~Bird();

    std::string makeSound()           const override;
    std::string getDiet()             const override;
    std::string getCareInstructions() const override;

    double getWingSpan() const;
    void printInfo()     const override;

private:
    double wingSpan; // в сантиметрах
}
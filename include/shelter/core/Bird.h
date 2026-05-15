#pragma once
#include "Pet.h"

class Bird : public Pet {
public:
    Bird(short id, const std::string& name, int age, double weight, double wingSpan);
    virtual ~Bird();

    std::string getType() const override { return "Bird"; }
    nlohmann::json toJson() const override;
    std::string makeSound()           const override;
    std::string getDiet()             const override;
    std::string getCareInstructions() const override;

    double getWingSpan() const;
    void printInfo()     const override;
    void printDetailInfo() const override;

private:
    double wingSpan;
};

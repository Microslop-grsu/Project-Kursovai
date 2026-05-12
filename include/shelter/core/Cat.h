#pragma once
#include "Pet.h"

class Cat : public Pet {
public:
    Cat(const std::string& name, int age, double weight, const std::string& furType);
    virtual ~Cat();

    std::string makeSound()           const override;
    std::string getDiet()             const override;
    std::string getCareInstructions() const override;

    std::string getFurType() const;
    void printInfo()         const override;

private:
    std::string furType; // например: "short", "long", "hairless"
};
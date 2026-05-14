#pragma once
#include "Pet.h"

class Cat : public Pet {
public:
    Cat(short id, const std::string& name, int age, double weight, const std::string& furType);
    virtual ~Cat();

    std::string getType() const override { return "Cat"; }
    nlohmann::json toJson() const override;
    std::string makeSound()           const override;
    std::string getDiet()             const override;
    std::string getCareInstructions() const override;

    std::string getFurType() const;
    void printInfo()         const override;

private:
    std::string furType;
};

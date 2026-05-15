#pragma once
#include "Pet.h"

class Dog : public Pet {
public:
    Dog(short id, const std::string& name, int age, double weight, bool knowsCommands);
    virtual ~Dog();

    std::string getType() const override { return "Dog"; }
    nlohmann::json toJson() const override;
    std::string makeSound()          const override;
    std::string getDiet()            const override;
    std::string getCareInstructions() const override;


    bool getKnowsCommands() const;

    void printInfo() const override;

    void printDetailInfo() const override;

private:
    bool knowsCommands;
};

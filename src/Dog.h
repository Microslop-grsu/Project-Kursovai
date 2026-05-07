#pragma once
#include "Pet.h"

class Dog : public Pet {
public:
    Dog(const std::string& name, int age, double weight, bool knowsCommands);
    virtual ~Dog();

    // Переопределяем чистые виртуальные методы из Pet
    std::string makeSound()          const override;
    std::string getDiet()            const override;
    std::string getCareInstructions() const override;

    // Геттер для уникального поля
    bool getKnowsCommands() const;

    void printInfo() const override;

private:
    bool knowsCommands;
};
#pragma once
#include <string>

class Pet {
public:
    Pet(const std::string &name, int age, double weight);

    virtual ~Pet();

    virtual std::string makeSound() const = 0;

    virtual std::string getDiet() const = 0;

    virtual std::string getCareInstructions() const = 0;

    std::string getName() const;

    int getAge() const;

    double getWeight() const;

    bool getIsHungry() const;


    void setIsHungry(bool hungry);


    virtual void printInfo() const;

protected:
    std::string name;
    int age;
    double weight;
    bool isHungry;
};

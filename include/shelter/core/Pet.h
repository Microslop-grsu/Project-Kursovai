#pragma once
#include <string>
#include "../utils/json.hpp"

class Pet {
public:
    Pet(short id, const std::string &name, int age, double weight);

    virtual ~Pet();

    virtual std::string makeSound() const = 0;
    virtual std::string getDiet() const = 0;
    virtual std::string getCareInstructions() const = 0;
    virtual std::string getType() const = 0;
    virtual nlohmann::json toJson() const;
    virtual void printInfo() const;
    virtual void printDetailInfo() const;

    short getId() const;
    std::string getName() const;
    int getAge() const;
    double getWeight() const;
    bool getIsHungry() const;

    void setIsHungry(bool hungry);

protected:
    short id;
    std::string name;
    int age;
    double weight;
    bool isHungry;
};

#pragma once
#include <string>
#include "../storage/json.hpp"

// Уровень активности животного
enum class ActivityLevel {
    LOW, NORMAL, HIGH
};

class Pet {
public:
    Pet(short id, const std::string &name, int age, double weight,
        ActivityLevel activity = ActivityLevel::NORMAL);

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
    ActivityLevel getActivityLevel() const;
    int getHungerLevel() const;
    int getHealthLevel() const;

    void setIsHungry(bool hungry);
    void setActivityLevel(ActivityLevel level);
    void setHungerLevel(int hunger);
    void setHealthLevel(int health);
    void increaseHunger(int amount = 1);
    void decreaseHunger(int amount);
    void changeHealth(int delta);

    static std::string activityToString(ActivityLevel level);

protected:
    short id;
    std::string name;
    int age;
    double weight;
    bool isHungry;
    ActivityLevel activityLevel;
    int hungerLevel;
    int healthLevel;

private:
    void syncHungerFlag();
};

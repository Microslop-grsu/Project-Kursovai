#pragma once
#include <string>

class Pet {
public:
    Pet(const std::string& name, int age, double weight);
    virtual ~Pet();

    // Чистые виртуальные методы — каждый вид реализует сам
    virtual std::string makeSound() const = 0;
    virtual std::string getDiet() const = 0;
    virtual std::string getCareInstructions() const = 0;

    // Геттеры
    std::string getName() const;
    int         getAge() const;
    double      getWeight() const;
    bool        getIsHungry() const;

    // Сеттер для состояния голода (Care Manager будет использовать)
    void setIsHungry(bool hungry);

    // Удобный вывод в лог
    virtual void printInfo() const;

protected:
    std::string name;
    int         age;
    double      weight;
    bool        isHungry;
};
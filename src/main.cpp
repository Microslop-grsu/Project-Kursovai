#include "Dog.h"
#include "Cat.h"
#include "Bird.h"
#include "Exotic.h"
#include <iostream>
#include <vector>

int main() {
    // Создаём всех животных через указатель на базовый класс
    std::vector<Pet*> shelter = {
        new Dog("Rex",    3, 25.5, true),
        new Cat("Luna",   2,  4.2, "long"),
        new Bird("Kesha", 1,  0.5, 30.0),
        new Exotic("Sly", 4,  2.1, 28.5)
    };

    std::cout << "=== Shelter Animals ===" << std::endl;

    for (Pet* pet : shelter) {
        std::cout << "\n--- " << pet->getName() << " ---" << std::endl;
        pet->printInfo();
        std::cout << "Sound:        " << pet->makeSound() << std::endl;
        std::cout << "Diet:         " << pet->getDiet() << std::endl;
        std::cout << "Care:         " << pet->getCareInstructions() << std::endl;
    }

    // Освобождаем память
    for (Pet* pet : shelter) {
        delete pet;
    }

    return 0;
}
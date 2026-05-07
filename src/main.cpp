#include <iostream>
#include <vector>
#include "Exotic.h"
#include "Dog.h"
#include "Cat.h"
#include "Bird.h"

#include "DataLoader.h"

int main() {
    std::vector<Pet*> pets;

    if (DataLoader::loadFromFile("pets.txt", pets)) {
        for (Pet* pet : pets) {
            pet->printInfo();
            delete pet;
        }
    } else {
        std::cout << "File not found!" << std::endl;
    }

    return 0;
}
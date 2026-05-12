#include <iostream>
#include <vector>
#include "../include/shelter/utils/DataLoader.h"

int main() {
    std::vector<Pet*> pets;

    if (DataLoader::loadFromFile("../data/pets.txt", pets)) {
        for (Pet* pet : pets) {
            pet->printInfo();
            delete pet;
        }
    } else {
        std::cout << "File not found!" << std::endl;
    }

    return 0;
}
// final check for github display

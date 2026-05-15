#include <iostream>
#include <vector>
#include "../include/shelter/utils/DataLoader.h"
#include "../include/shelter/utils/Logger.h"

int main() {
    Logger logger("../data/events.logs");
    std::vector<Pet*> pets;

    if (DataLoader::loadFromFile("../data/pets.json", pets)) {
        for (Pet* pet : pets) {
            pet->printInfo();
            delete pet;
        }
    } else {
        std::cout << "File not found!" << std::endl;
    }

    return 0;
}


#include <iostream>
#include <vector>
#include "../include/shelter/utils/DataLoader.h"
#include "../include/shelter/utils/Logger.h"

int main() {
    Logger logger("../data/events.logs");
    std::vector<Pet*> pets;

    if (DataLoader::loadFromFile("../data/pets.txt", pets)) {
        logger.debug("DATA", "DataLoader::loadFromFile() succeeded");
        for (Pet* pet : pets) {
            pet->printInfo();
            delete pet;
        }
    } else {
        logger.error("DATA", "DataLoader::loadFromFile() failed");
        std::cout << "File not found!" << std::endl;
    }

    return 0;
}


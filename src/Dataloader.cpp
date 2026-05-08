#include "Dataloader.h"
#include "Exotic.h"
#include "Dog.h"
#include "Cat.h"
#include "Bird.h"
#include <fstream>
#include <iostream>

bool DataLoader::loadFromFile(const std::string& path,
                              std::vector<Pet*>& outPets) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::string type, name, extra;
    int age;
    double weight;

    while (file >> type >> name >> age >> weight >> extra) {
        if (type == "Exotic") {
            double temp = std::stod(extra);
            outPets.push_back(new Exotic(name, age, weight, temp));
        }
        else if (type == "Dog") {
            bool commands = (extra == "yes");
            outPets.push_back(new Dog(name, age, weight, commands));
        }
        else if (type == "Cat") {
            outPets.push_back(new Cat(name, age, weight, extra));
        }
         else if (type == "Bird") {
            double wingSpan = std::stod(extra);
            outPets.push_back(new Bird(name, age, weight, wingSpan));
        }

    }
    return !outPets.empty();
}

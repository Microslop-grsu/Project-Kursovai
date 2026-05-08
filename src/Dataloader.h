#pragma once
#include <vector>
#include <string>
#include "Pet.h"

class DataLoader {
public:
    static bool loadFromFile(const std::string& path,
                             std::vector<Pet*>& outPets);
};
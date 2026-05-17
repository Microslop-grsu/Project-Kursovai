#pragma once
#include <string>
#include "../core/Pet.h"
#include "PetRepository.h"

class DataLoader {
private:
    std::string currentFilePath;

public:
    static bool loadFromFile(const std::string& path, PetRepository& repo);
    bool saveToFile(const std::string& path, const PetRepository& repo);

    void setCurrentFilePath(const std::string& path) {currentFilePath = path;}
};

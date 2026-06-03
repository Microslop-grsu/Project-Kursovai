#pragma once
#include <string>
#include "../core/Pet.h"
#include "PetRepository.h"
#include "../utils/Logger.h"

class DataLoader {
private:
    Logger logger;
    std::string currentFilePath;

public:
    explicit DataLoader(const std::string& logPath);

    bool loadFromFile(const std::string& path, PetRepository& repo);
    bool saveToFile(const std::string& path, const PetRepository& repo);

    void setCurrentFilePath(const std::string& path) {currentFilePath = path;}
};

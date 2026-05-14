#include "../../include/shelter/utils/DataLoader.h"
#include "../../include/shelter/utils/Logger.h"
#include "../../include/shelter/utils/json.hpp"
#include "../../include/shelter/core/Exotic.h"
#include "../../include/shelter/core/Dog.h"
#include "../../include/shelter/core/Cat.h"
#include "../../include/shelter/core/Bird.h"
#include <fstream>

using json = nlohmann::json;
Logger logger("../data/events.logs");

bool DataLoader::loadFromFile(const std::string& path,
                              std::vector<Pet*>& outPets) {
    std::ifstream file(path);
    if (!file.is_open()) {
        logger.error("FILE", "File not found");
        return false;
    }

    json data;
    try {
        file >> data;
    }   catch (json::parse_error& e) {
        logger.error("JSON", "JSON parse error: " + std::string(e.what()));
        return false;
    }
    repo.clear();

    if (!data.contains("pets") || !data["pets"].is_array()) {
        logger.error("PETS", "Pets not found");
        return false;
    }

    for (const auto& petJson: data["pets"] ) {
        if (auto pet = PetFactory::createFromJson(petJson)){
            repo.add(std::move(pet));
        }
        else {
            logger.debug("DATA", "Invalid JSON format");
            return false;
        }
    }
    currentFilePath = path;
    std::string msg = "Loaded " + std::to_string(repo.size()) + " pets from " + path;
    logger.debug("DATA", msg);
    return true;
}

bool DataLoader::saveToFile(const std::string &path, const PetRepository &repo) {
    json data = json::array();

    for (const auto& pet : repo.getAll()) {
        data.push_back(pet->toJson());
    }

    std::ofstream file(path);
    if (!file.is_open()) {
        logger.error("FILE", "Cannot write to file: " + path);
        return false;
    }

    file << data.dump(4);
    currentFilePath = path;
    std::string msg = "Saved " + std::to_string(repo.size()) + " pets to " + path;
    logger.info("DATA", msg);

    return true;
}

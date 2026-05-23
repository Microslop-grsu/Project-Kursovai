#include "../../include/shelter/storage/DataLoader.h"
#include "../../include/shelter/storage/PetFactory.h"
#include "../../include/shelter/storage/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace {
void logDataLoaderMessage(const std::string& level, const std::string& message) {
    std::cerr << "[DataLoader][" << level << "] " << message << '\n';
}
}

bool DataLoader::loadFromFile(const std::string& path, PetRepository& repo) {
    std::ifstream file(path);
    if (!file.is_open()) {
        logDataLoaderMessage("ERROR", "File not found: " + path);
        return false;
    }

    json data;
    try {
        file >> data;
    } catch (const json::parse_error& error) {
        logDataLoaderMessage("ERROR", "JSON parse error: " + std::string(error.what()));
        return false;
    }

    repo.clear();
    if (!data.contains("pets") || !data["pets"].is_array()) {
        logDataLoaderMessage("ERROR", "Pets array is missing.");
        return false;
    }

    for (const auto& petJson : data["pets"]) {
        auto pet = PetFactory::createFromJson(petJson);
        if (!pet) {
            logDataLoaderMessage("ERROR", "Invalid pet entry in " + path);
            return false;
        }
        repo.add(std::move(pet));
    }

    logDataLoaderMessage("INFO", "Loaded " + std::to_string(repo.size()) + " pets from " + path);
    return true;
}

bool DataLoader::saveToFile(const std::string& path, const PetRepository& repo) {
    json data;
    data["pets"] = json::array();

    for (const auto& pet : repo.getAll()) {
        data["pets"].push_back(pet->toJson());
    }

    std::ofstream file(path);
    if (!file.is_open()) {
        logDataLoaderMessage("ERROR", "Cannot write to file: " + path);
        return false;
    }

    file << data.dump(4);
    currentFilePath = path;
    logDataLoaderMessage("INFO", "Saved " + std::to_string(repo.size()) + " pets to " + path);
    return true;
}

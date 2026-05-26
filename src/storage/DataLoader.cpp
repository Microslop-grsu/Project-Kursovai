#include "../../include/shelter/storage/DataLoader.h"
#include "../../include/shelter/storage/PetFactory.h"
#include "../../include/shelter/storage/json.hpp"
#include "../../include/shelter/utils/Logger.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Logger dataLogger("../data/storage.logs");

bool DataLoader::loadFromFile(const std::string& path, PetRepository& repo) {
    std::ifstream file(path);
    if (!file.is_open()) {
        dataLogger.error("DATA", "Файл не найден: " + path);
        return false;
    }

    json data;
    try {
        file >> data;
    } catch (const json::parse_error& e) {
        dataLogger.error("DATA", "Ошибка парсинга JSON: " + std::string(e.what()));
        return false;
    }

    repo.clear();
    if (!data.contains("pets") || !data["pets"].is_array()) {
        dataLogger.error("DATA", "Данные о питомцах отсутствуют");
        return false;
    }

    for (const auto& petJson : data["pets"]) {
        auto pet = PetFactory::createFromJson(petJson);
        if (!pet) {
            dataLogger.error("DATA", "Неверная запись о питомце в" + path);
            return false;
        }
        repo.add(std::move(pet));
    }

    dataLogger.info("DATA", "Загружено " + std::to_string(repo.size()) + " питомцев из " + path);
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
        dataLogger.error("DATA", "Невозможно записать данные в : " + path);
        return false;
    }

    file << data.dump(4);
    currentFilePath = path;
    dataLogger.info("DATA", "Сохранено " + std::to_string(repo.size()) + " питомцев в " + path);
    return true;
}

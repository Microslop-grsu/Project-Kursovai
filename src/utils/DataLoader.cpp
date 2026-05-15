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

bool DataLoader::loadFromFile(const std::string &path,
                              std::vector<Pet *> &outPets)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        logger.error("FILE", "File not found");
        return false;
    }

    json data;
    try
    {
        file >> data;
    }
    catch (json::parse_error &e)
    {
        logger.error("JSON", "JSON parse error: " + std::string(e.what()));
        return false;
    }

    if (!data.contains("pets") || !data["pets"].is_array())
    {
        logger.error("PETS", "Pets not found");
        return false;
    }

    for (const auto &petJson : data["pets"])
    {
        if (!petJson.contains("type") || !petJson.contains("name") || !petJson.contains("age") || !petJson.contains("weight"))
        {
            continue;
        }

        std::string type = petJson["type"];
        std::string name = petJson["name"];
        int age = petJson["age"];
        double weight = petJson["weight"];

        if (type == "Exotic")
        {
            double temp = petJson.value("temperature", 25.0);
            double humidity = petJson.value("humidity", 50.0);
            outPets.push_back(new Exotic(name, age, weight, temp, humidity));
        }
        else if (type == "Dog")
        {
            bool hasCommands = petJson.value("hasCommands", false);
            outPets.push_back(new Dog(name, age, weight, hasCommands));
        }
        else if (type == "Cat")
        {
            std::string furType = petJson.value("furType", "Unkown");
            outPets.push_back(new Cat(name, age, weight, furType));
        }
        else if (type == "Bird")
        {
            if (petJson.contains("wingSpan"))
            {
                double wingSpan = petJson["wingSpan"];
                outPets.push_back(new Bird(name, age, weight, wingSpan));
            }
        }
    }

    logger.debug("DATA", "DataLoader::loadFromFile() succeeded");
    return !outPets.empty();
}
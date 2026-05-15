#include <iostream>
#include <vector>
#include "../include/shelter/utils/DataLoader.h"
#include "../include/shelter/utils/Logger.h"
#include "../include/shelter/core/MedicalRecord.h"
#include "../include/shelter/core/FeedingMonitor.h"
#include "../include/shelter/core/Exotic.h"

int main()
{
    Logger logger("../data/events.logs");
    std::vector<Pet *> pets;
    FeedingMonitor monitor;

    // 1. Загружаем питомцев
    if (DataLoader::loadFromFile("../data/pets.json", pets))
    {
        logger.debug("DATA", "DataLoader::loadFromFile() succeeded");

        // Регистрируем кормление и выводим информацию
        for (Pet *pet : pets)
        {
            monitor.feed(pet->getName());
            pet->printInfo();
        }

        // Проверка влажности для экзотических животных
        for (Pet *pet : pets)
        {
            Exotic *exotic = dynamic_cast<Exotic *>(pet);
            if (exotic)
            {
                exotic->checkHumidity(logger);
            }
        }
    }
    else
    {
        logger.error("DATA", "DataLoader::loadFromFile() failed");
        std::cout << "File not found!" << std::endl;
        return 1;
    }

    // 2. Работа с медицинскими записями
    MedicalRecord vaccineLog;
    vaccineLog.addVaccination("Bobby", "Rabies", "2025-01-10", true);
    vaccineLog.addVaccination("Murka", "Complex", "2024-01-01", true);

    if (vaccineLog.hasExpired())
    {
        std::cout << "\n[WARNING] Expired vaccinations found!" << std::endl;
        logger.warning("VACCINE", "Expired vaccinations found!");
    }

    for (const auto &rec : vaccineLog.getAllRecords())
    {
        std::cout << rec.petName << " - " << rec.vaccineName
                  << " (" << rec.date << ") - "
                  << (rec.isActive ? "Active" : "Expired") << std::endl;
    }

    // 3. Проверка голодающих
    monitor.checkStarvation(logger, pets);

    // Очистка памяти
    for (Pet *pet : pets)
    {
        delete pet;
    }

    return 0;
}
#include <iostream>
#include <vector>
#include "../include/shelter/utils/DataLoader.h"
#include "../include/shelter/utils/Logger.h"
#include "../include/shelter/core/MedicalRecord.h"
#include "../include/shelter/core/FeedingMonitor.h"
#include "../include/shelter/core/Exotic.h"

short menu() {
    short ans = 0;
    std::cout << "=======================" << std::endl;
    std::cout << "   ShelterCore System  " << std::endl;
    std::cout << "Меню управления приютом" << std::endl;
    std::cout << "=======================" << std::endl;
    std::cout << "Выберите пункт:        " << std::endl;
    std::cout << "1 - Вывести список всех животных" << std::endl;
    std::cout << "2 - Детальная информация о животном по id" << std::endl;
    std::cout << "0 - Выход " << std::endl;
    std::cout << "> "; std::cin >> ans;

    return ans;
}

void printPet(const std::vector<Pet*> pets) {
    for (auto pet : pets) {
        pet->printInfo();
    }
}

void delete_buffer(std::vector<Pet*> pets) {
    for (auto pet : pets) {
        delete pet;
    }
    std::cout << std::endl;
}

int main()
{
    Logger logger("../data/events.logs");
    std::vector<Pet *> pets;
    FeedingMonitor monitor;

    short answer = 0;

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
    if (DataLoader::loadFromFile("../data/pets.json", pets)) {
        logger.debug("DATA", "DataLoader::loadFromFile() succeeded");

    } else {
        std::cout << "File not found!" << std::endl;
        return 1;
    }

    // 2. Работа с медицинскими записями
    MedicalRecord vaccineLog;
    vaccineLog.addVaccination("Bobby", "Rabies", "2025-01-10", true);
    vaccineLog.addVaccination("Murka", "Complex", "2024-01-01", true);


    // 3. Проверка голодающих
    monitor.checkStarvation(logger, pets);

    bool exit = false;
    do {
        answer = menu();
        logger.debug("MENU", "Menu answer: " + std::to_string(answer));
        switch (answer) {
            case 1: printPet(pets); break;
            case 0: exit = true; delete_buffer(pets); break;
        }
    }   while (!exit);



    return 0;
}

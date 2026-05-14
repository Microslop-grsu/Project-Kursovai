#include <iostream>
#include <vector>
#include <memory>
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

int main() {
    Logger logger("../data/events.logs");
    DataLoader loader;
    PetRepository repo;

    std::vector<Pet *> pets;
    FeedingMonitor monitor;

    short answer = 0;

    loader.loadFromFile("../data/pets.json", repo);

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
        switch (answer) {
            case 1:

                logger.debug("MENU", "Показать список животных");
                break;
            case 0:
                exit = true;

                logger.debug("DATA", "pets deleted");
                break;
        }
    }   while (!exit);



    return 0;
}


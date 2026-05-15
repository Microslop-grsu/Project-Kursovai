#include <iostream>
#include <vector>
#include <memory>
#include "../include/shelter/utils/DataLoader.h"
#include "../include/shelter/utils/Logger.h"
#include "../include/shelter/core/MedicalRecord.h"
#include "../include/shelter/core/FeedingMonitor.h"
#include "../include/shelter/core/Exotic.h"

short menu();

void print_pet(const std::vector<std::unique_ptr<Pet>>& pets);

void printDetailInfo(Pet* pet);

int main() {
    Logger logger("../data/events.logs");
    DataLoader loader;
    PetRepository repo;
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
            case 2:
                short id;
                std::cout << "Введите id животного >>> "; std::cin >> id;
                printDetailInfo(repo.findByID(id));
                logger.debug("MENU", "Показал детальную информацию о id - "+ std::to_string(id));
                break;
            case 1:

                print_pet(repo.getAll());
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

void print_pet(const std::vector<std::unique_ptr<Pet>>& pets) {
    for (const auto& pet : pets) {
        pet->printInfo();
    }
    std::cout << std::endl;
}

void printDetailInfo(Pet* pet) {
    if (pet){
        pet->printDetailInfo();
    }
    else {
        std::cout << "Такого id нет в базе"<< std::endl;
    }

}

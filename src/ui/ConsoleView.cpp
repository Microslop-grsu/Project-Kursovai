#include "../../include/shelter/ui/ConsoleView.h"
#include "../../include/shelter/services/ShelterManager.h"
#include "../../include/shelter/services/DietCalculator.h"
#include "shelter/services/SimulationEngine.h"
#include "../../include/shelter/core/Bird.h"
#include "../../include/shelter/core/Cat.h"
#include "../../include/shelter/core/Dog.h"
#include "../../include/shelter/core/Exotic.h"
#include <limits>
#include <iostream>
#include <memory>
#include <string>

ConsoleView::ConsoleView(std::shared_ptr<ShelterManager> m) : manager(m) {}

void ConsoleView::run() {
    bool exit = false;
    short ans = 0;
    while (!exit) {
        std::cout << "\n=== ShelterCore System ===\n";
        std::cout << "1. Режим симуляции\n";
        std::cout << "2. Интерактивный режим\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите режим >>> "; std::cin >> ans;
        switch (ans) {
            case 1: simulationMode(); break;
            case 2: interactiveMode(); break;
            case 0: exit = true; break;
            default: std::cout << "Неверный пункт\n"; break;
        }
    }
}


void ConsoleView::interactiveMode() {
    bool exit = false;
    short ans = 0;
    while (!exit) {
        std::cout << "\n=== ShelterCore System ===\n";
        std::cout << "1. Список всех животных\n";
        std::cout << "2. Принять питомца\n";
        std::cout << "3. Найти по критериям\n";
        std::cout << "4. Отдать питомца\n";
        std::cout << "5. Детальная информация по ID\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите режим >>> "; std::cin >> ans;
        switch (ans) {
            case 1: listAllPets(); break;
            case 2: addPetForm(); break;
            case 3: searchPets(); break;
            case 4: deletePetForm(); break;
            case 5: showPetDetails(); break;
            case 0: exit = true; break;
            default: std::cout << "Неверный пункт\n"; break;
        }
    }
}

void ConsoleView::simulationMode() {
    std::cout << "\n=== Режим симуляции ===\n";
    const int totalTicks = readPositiveInteger("Количество тиков", 100);
    const int displayInterval = readPositiveInteger("Интервал отображения", 10);

    SimulationConfig config;
    SimulationEngine engine(manager, config);
    engine.run(totalTicks, displayInterval);
}


const void ConsoleView::listAllPets() {
    for (const auto& pet : manager->getRepo().getAll()) {
        pet->printInfo();
    }
}

void ConsoleView::showPetDetails() {
    short id;
    std::cout << "ID >>> "; std::cin >> id;
    Pet* pet = manager->getRepo().findByID(id);
    if (pet) {
        pet->printDetailInfo();
        printVaccinations(id);
        const auto issues = manager->getMedicalRecord().getHealthIssuesForPet(id);
        if (!issues.empty()) {
            std::cout << "Болезни:\n";
            for (const auto& issue : issues)
                std::cout << "- " << issue << "\n";
        } else {
            std::cout << "Болезни: нет\n";
        }

        std::cout << "Суточная норма: " << DietCalculator::calculateDailyGrams(*pet, manager->getMedicalRecord()) << " г\n";

    } else {
        std::cout << "Не найден\n";
    }
}

void ConsoleView::printVaccinations(short petId) {
    std::cout << "Прививки:\n";
    bool found = false;
    for (const auto& rec : manager->getMedicalRecord().getAllRecords()) {
        if (rec.petId == petId) {
            std::cout << "- " << rec.vaccineName << " [" << rec.date << "]\n";
            found = true;
        }
    }
    if (!found) std::cout << "- нет записей\n";
}

void ConsoleView::searchPets() {
    PetRepository::SearchCriteria crit;
    std::string val;
    int num = -1;

    std::cout << "Тип (Dog/Cat/Bird/Exotic или -): ";
    std::cin >> val;
    if (val != "-") {
        crit.type = val;
    }

    std::cout << "Мин. возраст (-1 для пропуска): ";
    std::cin >> num;
    if (num >= 0) {
        crit.min_age = num;
    }

    std::cout << "Макс. возраст (-1 для пропуска): ";
    std::cin >> num;
    if (num >= 0) {
        crit.max_age = num;
    }

    std::cout << "Мин. вес (-1 для пропуска): ";
    std::cin >> num;
    if (num >= 0) {
        crit.min_weight = num;
    }

    std::cout << "Макс. вес (-1 для пропуска): ";
    std::cin >> num;
    if (num >= 0) {
        crit.max_weight = num;
    }

    auto found = manager->getRepo().findByCriteria(crit);
    if (found.empty()) {
        std::cout << "Ничего не найдено\n";
        return;
    }
    for (auto p : found) {
        p->printInfo();
    }
}

int readPositiveInteger(const std::string& prompt, int defaultValue) {
    std::cout << prompt << " [" << defaultValue << "]: ";

    int value = defaultValue;
    if (!(std::cin >> value) || value <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return defaultValue;
    }

    return value;
}

void ConsoleView::addPetForm() {
    std::cout << "=== Форма добавления питомца ===\n";

    std::string type;
    std::string name;
    int age = 0;
    double weight = 0.0;
    short id = manager->generateNextPetId();

    std::cout << "Введите тип (Dog/Cat/Bird/Exotic): ";
    std::cin >> type;

    if (type != "Dog" && type != "Cat" && type != "Bird" && type != "Exotic") {
        std::cout << "Неправильный тип животного\n";
        return;
    }

    std::cout << "Введите имя: ";
    std::cin >> name;

    std::cout << "Введите возраст: ";
    if (!(std::cin >> age)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Некорректный возраст\n";
        return;
    }

    std::cout << "Введите вес: ";
    if (!(std::cin >> weight)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Некорректный вес\n";
        return;
    }

    std::unique_ptr<Pet> pet;

    if (type == "Dog") {
        char knowsCommands = 'n';
        std::cout << "Знает команды? (y/n): ";
        std::cin >> knowsCommands;
        pet = std::make_unique<Dog>(id, name, age, weight, knowsCommands == 'y' || knowsCommands == 'Y');
    } else if (type == "Cat") {
        std::string furType;
        std::cout << "Тип шерсти (short/long/hairless): ";
        std::cin >> furType;
        pet = std::make_unique<Cat>(id, name, age, weight, furType);
    } else if (type == "Bird") {
        double wingSpan = 0.0;
        std::cout << "Размах крыльев: ";
        if (!(std::cin >> wingSpan)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Некорректный размах крыльев\n";
            return;
        }
        pet = std::make_unique<Bird>(id, name, age, weight, wingSpan);
    } else if (type == "Exotic") {
        double temperature = 0.0;
        double humidity = 50.0;

        std::cout << "Требуемая температура: ";
        if (!(std::cin >> temperature)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Некорректная температура\n";
            return;
        }

        std::cout << "Влажность (по умолчанию 50): ";
        if (!(std::cin >> humidity)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            humidity = 50.0;
        }

        pet = std::make_unique<Exotic>(id, name, age, weight, temperature, humidity);
    }

    if (!pet) {
        std::cout << "Не удалось создать питомца\n";
        return;
    }

    if (manager->addPet(std::move(pet))) {
        std::cout << "Питомец добавлен и сохранён в файл\n";
        listAllPets();
    } else {
        std::cout << "Не удалось добавить питомца\n";
    }
}

void ConsoleView::deletePetForm() {
    short id;
    std::cout << "Введите ID питомца для удаления: ";
    if (!(std::cin >> id)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Некорректный ID\n";
    }
    manager->removePet(id);
}

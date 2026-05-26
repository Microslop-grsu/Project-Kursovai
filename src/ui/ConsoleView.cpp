#include "../../include/shelter/ui/ConsoleView.h"
#include "../../include/shelter/services/ShelterManager.h"
#include "../../include/shelter/services/DietCalculator.h"
#include <iostream>
#include <vector>

ConsoleView::ConsoleView(ShelterManager& m) : manager(m) {}

void ConsoleView::run() {
    bool exit = false;
    while (!exit) {
        short choice = showMenu();
        switch (choice) {
            case 1: listAllPets(); break;
            case 2: showPetDetails(); break;
            case 3: searchPets(); break;
            case 0: exit = true; break;
            default: std::cout << "Неверный пункт\n"; break;
        }
    }
}

short ConsoleView::showMenu() {
    short ans = 0;
    std::cout << "\n=== ShelterCore System ===\n";
    std::cout << "1 - Список животных\n";
    std::cout << "2 - Детали по ID\n";
    std::cout << "3 - Поиск\n";
    std::cout << "0 - Выход\n> ";
    std::cin >> ans;
    return ans;
}

void ConsoleView::listAllPets() {
    for (const auto& pet : manager.getRepo().getAll()) {
        pet->printInfo();
    }
}

void ConsoleView::showPetDetails() {
    short id;
    std::cout << "ID >>> "; std::cin >> id;
    Pet* pet = manager.getRepo().findByID(id);
    if (pet) {
        pet->printDetailInfo();
        printVaccinations(id);
        const auto& issues = manager.getMedicalRecord().getHealthIssues();
        if (!issues.empty()) {
            std::cout << "Болезни:\n";
            for (const auto& issue : issues)
                std::cout << "- " << issue << "\n";
        } else {
            std::cout << "Болезни: нет\n";
        }

        std::cout << "Суточная норма: " << DietCalculator::calculateDailyGrams(*pet, manager.getMedicalRecord()) << " г\n";

    } else {
        std::cout << "Не найден\n";
    }
}

void ConsoleView::printVaccinations(short petId) {
    std::cout << "Прививки:\n";
    bool found = false;
    for (const auto& rec : manager.getMedicalRecord().getAllRecords()) {
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

    auto found = manager.getRepo().findByCriteria(crit);
    if (found.empty()) {
        std::cout << "Ничего не найдено\n";
        return;
    }
    for (auto p : found) {
        p->printInfo();
    }
}

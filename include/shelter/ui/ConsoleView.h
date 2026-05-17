#pragma once
class ShelterManager;

class ConsoleView {
public:
    ConsoleView(ShelterManager& manager);
    void run();

private:
    ShelterManager& manager;

    short showMenu();
    void listAllPets();
    void showPetDetails();
    void searchPets();
    void printVaccinations(short petId);
};



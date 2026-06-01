#pragma once
#include <string>
#include <memory>
class ShelterManager;

class ConsoleView {
public:
    ConsoleView(std::shared_ptr<ShelterManager> manager);
    void run();

private:
    std::shared_ptr<ShelterManager> manager;

    const void listAllPets();
    void addPetForm();
    void deletePetForm();
    void showPetDetails();
    void searchPets();
    void printVaccinations(short petId);
    void simulationMode();
    void interactiveMode();
};

int readPositiveInteger(const std::string& prompt, int defaultValue);



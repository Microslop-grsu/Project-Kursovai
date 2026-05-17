#include "shelter/services/ShelterManager.h"
#include "shelter/ui/ConsoleView.h"

int main() {
    ShelterManager manager("../data/events.logs");

    if (!manager.loadData("../data/pets.json")) {
        return 1;
    }

    manager.addVaccination(1, "Rabies", "2025-01-10");
    manager.checkHungryPets();

    ConsoleView view(manager);
    view.run();

    return 0;
}

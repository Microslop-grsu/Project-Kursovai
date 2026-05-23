#include "shelter/services/ShelterManager.h"
#include "shelter/services/SimulationEngine.h"
#include "shelter/ui/ConsoleView.h"
#include <filesystem>
#include <iostream>
#include <limits>
#include <memory>

namespace {
std::filesystem::path findProjectRoot() {
    std::filesystem::path current = std::filesystem::current_path();
    for (int depth = 0; depth < 4; ++depth) {
        if (std::filesystem::exists(current / "data" / "pets.json")
            && std::filesystem::exists(current / "include")) {
            return current;
        }

        if (!current.has_parent_path()) {
            break;
        }
        current = current.parent_path();
    }

    return std::filesystem::current_path();
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
}

int main() {
    const std::filesystem::path projectRoot = findProjectRoot();
    const std::filesystem::path petsPath = projectRoot / "data" / "pets.json";
    const std::filesystem::path logPath = projectRoot / "data" / "events.logs";
    const std::filesystem::path statsPath = projectRoot / "stats.json";

    auto manager = std::make_shared<ShelterManager>(logPath.string());
    if (!manager->loadData(petsPath.string())) {
        std::cerr << "Failed to load pet data from " << petsPath << '\n';
        return 1;
    }

    std::cout << "\n=== ShelterCore ===\n";
    std::cout << "1 - Simulation mode\n";
    std::cout << "2 - Interactive mode\n";
    std::cout << "0 - Exit\n> ";

    int choice = 0;
    if (!(std::cin >> choice)) {
        return 1;
    }

    if (choice == 1) {
        const int totalTicks = readPositiveInteger("Number of ticks", 100);
        const int displayInterval = readPositiveInteger("Display interval", 10);

        SimulationConfig config;
        SimulationEngine engine(manager, config);
        engine.run(totalTicks, displayInterval);
        engine.saveStatistics(statsPath.string());

        std::cout << "Statistics saved to " << statsPath << '\n';
        return 0;
    }

    if (choice == 2) {
        ConsoleView view(*manager);
        view.run();
        return 0;
    }

    return 0;
}

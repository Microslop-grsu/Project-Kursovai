#include "shelter/services/ShelterManager.h"

#include "shelter/ui/ConsoleView.h"
#include <filesystem>
#include <iostream>
#include <memory>
#ifdef _WIN32
#include <windows.h>
#endif


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


}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    const std::filesystem::path projectRoot = findProjectRoot();
    const std::filesystem::path petsPath = projectRoot / "data" / "pets.json";
    const std::filesystem::path logPath = projectRoot / "data" / "events.logs";
    const std::filesystem::path statsPath = projectRoot / "stats.json";

    auto manager = std::make_shared<ShelterManager>(logPath.string(), petsPath.string());
    if (!manager->loadData(petsPath.string())) {
        std::cerr << "Failed to load pet data from " << petsPath << '\n';
        return 1;
    }

    ConsoleView view(manager);
    view.run();

    return 0;
}

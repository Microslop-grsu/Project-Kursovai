#include "../../include/shelter/services/EventGenerator.h"
#include "../../include/shelter/core/Pet.h"
#include <array>
#include <ctime>

namespace {
template <typename T, std::size_t N>
const T& randomFrom(const std::array<T, N>& values, std::mt19937& engine) {
    std::uniform_int_distribution<std::size_t> distribution(0, values.size() - 1);
    return values[distribution(engine)];
}

int randomInt(std::mt19937& engine, int minValue, int maxValue) {
    std::uniform_int_distribution<int> distribution(minValue, maxValue);
    return distribution(engine);
}

double randomDouble(std::mt19937& engine, double minValue, double maxValue) {
    std::uniform_real_distribution<double> distribution(minValue, maxValue);
    return distribution(engine);
}

ActivityLevel randomActivityLevel(std::mt19937& engine) {
    const std::array<ActivityLevel, 3> levels = {
        ActivityLevel::LOW,
        ActivityLevel::NORMAL,
        ActivityLevel::HIGH
    };
    return randomFrom(levels, engine);
}

std::string vaccineForType(const std::string& type) {
    if (type == "Dog") {
        return "Rabies";
    }
    if (type == "Cat") {
        return "Feline Distemper";
    }
    if (type == "Bird") {
        return "Avian Flu";
    }
    return "Exotic Wellness";
}
}

EventGenerator::EventGenerator(const SimulationConfig& config)
    : config(config), randomEngine(std::random_device{}()) {}

std::vector<std::shared_ptr<SimulationEvent>> EventGenerator::generateEvents(
    int currentTick,
    const std::vector<Pet*>& animals
) {
    (void)currentTick;

    std::vector<std::shared_ptr<SimulationEvent>> events;
    if (shouldEventOccur(config.arrivalProbability)) {
        events.push_back(createAnimalArrival());
    }

    if (animals.empty()) {
        return events;
    }

    auto pickPet = [&]() -> Pet& {
        std::uniform_int_distribution<std::size_t> distribution(0, animals.size() - 1);
        return *animals[distribution(randomEngine)];
    };

    if (shouldEventOccur(config.vaccinationProbability)) {
        events.push_back(createVaccination(pickPet()));
    }

    if (shouldEventOccur(config.vetCheckProbability)) {
        events.push_back(createVetCheck(pickPet()));
    }

    if (shouldEventOccur(config.adoptionProbability)) {
        events.push_back(createAnimalAdoption(pickPet()));
    }

    return events;
}

void EventGenerator::setConfig(const SimulationConfig& newConfig) {
    config = newConfig;
}

SimulationConfig EventGenerator::getConfig() const {
    return config;
}

bool EventGenerator::shouldEventOccur(float probability) {
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    return distribution(randomEngine) < probability;
}

std::shared_ptr<SimulationEvent> EventGenerator::createAnimalArrival() {
    ArrivingPetData petData = createArrivalData();
    return std::make_shared<AnimalArrivedEvent>(petData, "New intake");
}

std::shared_ptr<SimulationEvent> EventGenerator::createAnimalAdoption(Pet& pet) {
    return std::make_shared<AnimalAdoptedEvent>(pet.getId(), pet.getName(), "Matched with an adopter");
}

std::shared_ptr<SimulationEvent> EventGenerator::createVaccination(Pet& pet) {
    return std::make_shared<VaccinationEvent>(
        pet.getId(),
        pet.getName(),
        vaccineForType(pet.getType()),
        createDateString(std::time(nullptr)),
        "Routine vaccination"
    );
}

std::shared_ptr<SimulationEvent> EventGenerator::createVetCheck(Pet& pet) {
    return std::make_shared<VetCheckEvent>(pet.getId(), pet.getName(), "Scheduled health review");
}

ArrivingPetData EventGenerator::createArrivalData() {
    const std::array<std::string, 4> types = {"Dog", "Cat", "Bird", "Exotic"};
    const std::string& selectedType = randomFrom(types, randomEngine);

    ArrivingPetData petData;
    petData.type = selectedType;
    petData.activityLevel = randomActivityLevel(randomEngine);

    if (selectedType == "Dog") {
        const std::array<std::string, 6> names = {"Max", "Bella", "Luna", "Rocky", "Charlie", "Buddy"};
        petData.name = randomFrom(names, randomEngine);
        petData.age = randomInt(randomEngine, 1, 10);
        petData.weight = randomDouble(randomEngine, 8.0, 35.0);
        petData.knowsCommands = randomInt(randomEngine, 0, 1) == 1;
    } else if (selectedType == "Cat") {
        const std::array<std::string, 6> names = {"Murka", "Lily", "Milo", "Loki", "Misty", "Nora"};
        const std::array<std::string, 4> furTypes = {"short", "long", "hairless", "striped"};
        petData.name = randomFrom(names, randomEngine);
        petData.age = randomInt(randomEngine, 1, 12);
        petData.weight = randomDouble(randomEngine, 2.5, 7.0);
        petData.furType = randomFrom(furTypes, randomEngine);
    } else if (selectedType == "Bird") {
        const std::array<std::string, 5> names = {"Kesha", "Rio", "Kiwi", "Sky", "Sunny"};
        petData.name = randomFrom(names, randomEngine);
        petData.age = randomInt(randomEngine, 1, 8);
        petData.weight = randomDouble(randomEngine, 0.2, 2.0);
        petData.wingSpan = randomDouble(randomEngine, 15.0, 70.0);
    } else {
        const std::array<std::string, 5> names = {"Iggy", "Echo", "Nova", "Mango", "Zuri"};
        petData.name = randomFrom(names, randomEngine);
        petData.age = randomInt(randomEngine, 1, 15);
        petData.weight = randomDouble(randomEngine, 0.8, 12.0);
        petData.temperature = randomDouble(randomEngine, 24.0, 32.0);
        petData.humidity = randomDouble(randomEngine, 45.0, 65.0);
    }

    return petData;
}

std::string EventGenerator::createDateString(std::time_t timestamp) const {
    std::tm timeInfo {};
#if defined(_WIN32)
    localtime_s(&timeInfo, &timestamp);
#else
    localtime_r(&timestamp, &timeInfo);
#endif

    char buffer[11] = {};
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeInfo);
    return buffer;
}

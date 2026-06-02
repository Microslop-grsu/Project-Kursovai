#pragma once
#include <ctime>
#include <string>
#include <vector>
#include "../core/Pet.h"

enum class EventType
{
    ANIMAL_ARRIVED,
    ANIMAL_ADOPTED,
    VACCINATION,
    VET_CHECK,
    FEEDING,
    HEALTH_UPDATE,
    UNKNOWN
};

std::string eventTypeToString(EventType type);

struct ArrivingPetData
{
    std::string type;
    std::string name;
    int age = 0;
    double weight = 0.0;
    ActivityLevel activityLevel = ActivityLevel::NORMAL;
    bool knowsCommands = false;
    std::string furType = "Unknown";
    double wingSpan = 0.0;
    double temperature = 0.0;
    double humidity = 50.0;
};

class SimulationEvent
{
public:
    SimulationEvent(EventType type, int petId = -1, const std::string &description = "");
    virtual ~SimulationEvent() = default;

    EventType getType() const;
    int getPetId() const;
    const std::string &getDescription() const;
    std::time_t getTimestamp() const;

    virtual std::string toString() const;

private:
    EventType type;
    int petId;
    std::string description;
    std::time_t timestamp;
};

class AnimalArrivedEvent : public SimulationEvent
{
public:
    AnimalArrivedEvent(const ArrivingPetData &petData, const std::vector<std::string> &healthIssues, const std::string &description = "");

    const ArrivingPetData &getPetData() const;
    const std::vector<std::string> &getHealthIssues() const;
    std::string toString() const override;

private:
    ArrivingPetData petData;
    std::vector<std::string> healthIssues;
};

class AnimalAdoptedEvent : public SimulationEvent
{
public:
    AnimalAdoptedEvent(int petId, std::string petName, const std::string &description = "");

    const std::string &getPetName() const;
    std::string toString() const override;

private:
    std::string petName;
};

class VaccinationEvent : public SimulationEvent
{
public:
    VaccinationEvent(int petId, std::string petName, std::string vaccineName, std::string date, const std::string &description = "");

    const std::string &getPetName() const;
    const std::string &getVaccineName() const;
    const std::string &getDate() const;
    std::string toString() const override;

private:
    std::string petName;
    std::string vaccineName;
    std::string date;
};

class VetCheckEvent : public SimulationEvent
{
public:
    VetCheckEvent(int petId, std::string petName, const std::string &description = "");

    const std::string &getPetName() const;
    std::string toString() const override;

private:
    std::string petName;
};

class FeedingEvent : public SimulationEvent
{
public:
    FeedingEvent(int petId, std::string petName, double grams, int previousHunger, int newHunger);

    std::string toString() const override;

private:
    std::string petName;
    double grams;
    int previousHunger;
    int newHunger;
};

class HealthUpdateEvent : public SimulationEvent
{
public:
    HealthUpdateEvent(int petId, std::string petName, int previousHealth, int newHealth, std::string reason);

    std::string toString() const override;

private:
    std::string petName;
    int previousHealth;
    int newHealth;
    std::string reason;
};
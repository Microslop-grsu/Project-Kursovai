#include "../../include/shelter/services/SimulationEvent.h"
#include <iomanip>
#include <sstream>
#include <utility>

namespace
{
    std::string formatWeight(double weight)
    {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(1) << weight;
        return stream.str();
    }

    std::string formatPetIdentity(const std::string &petName, int petId)
    {
        std::ostringstream stream;
        stream << petName;
        if (petId >= 0)
        {
            stream << " (#" << petId << ")";
        }
        return stream.str();
    }
}

std::string eventTypeToString(EventType type)
{
    switch (type)
    {
    case EventType::ANIMAL_ARRIVED:
        return "Animal Arrived";
    case EventType::ANIMAL_ADOPTED:
        return "Animal Adopted";
    case EventType::VACCINATION:
        return "Vaccination";
    case EventType::VET_CHECK:
        return "Vet Check";
    case EventType::FEEDING:
        return "Feeding";
    case EventType::HEALTH_UPDATE:
        return "Health Update";
    default:
        return "Unknown";
    }
}

SimulationEvent::SimulationEvent(EventType type, int petId, const std::string &description)
    : type(type), petId(petId), description(description), timestamp(std::time(nullptr)) {}

EventType SimulationEvent::getType() const
{
    return type;
}

int SimulationEvent::getPetId() const
{
    return petId;
}

const std::string &SimulationEvent::getDescription() const
{
    return description;
}

std::time_t SimulationEvent::getTimestamp() const
{
    return timestamp;
}

std::string SimulationEvent::toString() const
{
    std::ostringstream stream;
    stream << eventTypeToString(type);
    if (petId >= 0)
    {
        stream << " [pet #" << petId << "]";
    }
    if (!description.empty())
    {
        stream << ": " << description;
    }
    return stream.str();
}

AnimalArrivedEvent::AnimalArrivedEvent(const ArrivingPetData &petData, const std::vector<std::string> &healthIssues, const std::string &description)
    : SimulationEvent(EventType::ANIMAL_ARRIVED, -1, description), petData(petData), healthIssues(healthIssues) {}

const ArrivingPetData &AnimalArrivedEvent::getPetData() const
{
    return petData;
}

const std::vector<std::string> &AnimalArrivedEvent::getHealthIssues() const
{
    return healthIssues;
}

std::string AnimalArrivedEvent::toString() const
{
    std::ostringstream stream;
    stream << eventTypeToString(getType()) << ": "
           << petData.type << " " << petData.name
           << ", age " << petData.age
           << ", weight " << formatWeight(petData.weight) << "kg";
    if (!healthIssues.empty())
    {
        stream << ", diseases: ";
        for (size_t i = 0; i < healthIssues.size(); ++i)
        {
            if (i > 0)
                stream << ", ";
            stream << healthIssues[i];
        }
    }
    else
    {
        stream << ", diseases: none";
    }
    if (!getDescription().empty())
    {
        stream << " (" << getDescription() << ")";
    }
    return stream.str();
}

AnimalAdoptedEvent::AnimalAdoptedEvent(int petId, std::string petName, const std::string &description)
    : SimulationEvent(EventType::ANIMAL_ADOPTED, petId, description), petName(std::move(petName)) {}

const std::string &AnimalAdoptedEvent::getPetName() const
{
    return petName;
}

std::string AnimalAdoptedEvent::toString() const
{
    std::ostringstream stream;
    stream << eventTypeToString(getType()) << ": "
           << formatPetIdentity(petName, getPetId())
           << " found a new home";
    if (!getDescription().empty())
    {
        stream << " (" << getDescription() << ")";
    }
    return stream.str();
}

VaccinationEvent::VaccinationEvent(int petId, std::string petName, std::string vaccineName, std::string date, const std::string &description)
    : SimulationEvent(EventType::VACCINATION, petId, description),
      petName(std::move(petName)),
      vaccineName(std::move(vaccineName)),
      date(std::move(date)) {}

const std::string &VaccinationEvent::getPetName() const
{
    return petName;
}

const std::string &VaccinationEvent::getVaccineName() const
{
    return vaccineName;
}

const std::string &VaccinationEvent::getDate() const
{
    return date;
}

std::string VaccinationEvent::toString() const
{
    std::ostringstream stream;
    stream << eventTypeToString(getType()) << ": "
           << formatPetIdentity(petName, getPetId())
           << " received " << vaccineName
           << " on " << date;
    if (!getDescription().empty())
    {
        stream << " (" << getDescription() << ")";
    }
    return stream.str();
}

VetCheckEvent::VetCheckEvent(int petId, std::string petName, const std::string &description)
    : SimulationEvent(EventType::VET_CHECK, petId, description), petName(std::move(petName)) {}

const std::string &VetCheckEvent::getPetName() const
{
    return petName;
}

std::string VetCheckEvent::toString() const
{
    std::ostringstream stream;
    stream << eventTypeToString(getType()) << ": "
           << formatPetIdentity(petName, getPetId());
    if (!getDescription().empty())
    {
        stream << " - " << getDescription();
    }
    return stream.str();
}

FeedingEvent::FeedingEvent(int petId, std::string petName, double grams, int previousHunger, int newHunger)
    : SimulationEvent(EventType::FEEDING, petId, ""),
      petName(std::move(petName)),
      grams(grams),
      previousHunger(previousHunger),
      newHunger(newHunger) {}

std::string FeedingEvent::toString() const
{
    std::ostringstream stream;
    stream << eventTypeToString(getType()) << ": "
           << formatPetIdentity(petName, getPetId())
           << " (" << previousHunger << " -> " << newHunger
           << ", " << std::fixed << std::setprecision(1) << grams << "g)";
    return stream.str();
}

HealthUpdateEvent::HealthUpdateEvent(int petId, std::string petName, int previousHealth, int newHealth, std::string reason)
    : SimulationEvent(EventType::HEALTH_UPDATE, petId, ""),
      petName(std::move(petName)),
      previousHealth(previousHealth),
      newHealth(newHealth),
      reason(std::move(reason)) {}

std::string HealthUpdateEvent::toString() const
{
    std::ostringstream stream;
    stream << eventTypeToString(getType()) << ": "
           << formatPetIdentity(petName, getPetId())
           << " (" << previousHealth << " -> " << newHealth << ")";
    if (!reason.empty())
    {
        stream << " - " << reason;
    }
    return stream.str();
}
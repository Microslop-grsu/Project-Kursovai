#include "../../include/shelter/services/EventProcessor.h"
#include "../../include/shelter/core/Bird.h"
#include "../../include/shelter/core/Cat.h"
#include "../../include/shelter/core/Dog.h"
#include "../../include/shelter/core/Exotic.h"
#include "../../include/shelter/services/ShelterManager.h"
#include <utility>

namespace
{
    std::unique_ptr<Pet> createPetFromArrivalData(const ArrivingPetData &petData, short petId)
    {
        if (petData.type == "Dog")
        {
            auto pet = std::make_unique<Dog>(petId, petData.name, petData.age, petData.weight, petData.knowsCommands);
            pet->setActivityLevel(petData.activityLevel);
            return pet;
        }

        if (petData.type == "Cat")
        {
            auto pet = std::make_unique<Cat>(petId, petData.name, petData.age, petData.weight, petData.furType);
            pet->setActivityLevel(petData.activityLevel);
            return pet;
        }

        if (petData.type == "Bird")
        {
            auto pet = std::make_unique<Bird>(petId, petData.name, petData.age, petData.weight, petData.wingSpan);
            pet->setActivityLevel(petData.activityLevel);
            return pet;
        }

        if (petData.type == "Exotic")
        {
            auto pet = std::make_unique<Exotic>(
                petId,
                petData.name,
                petData.age,
                petData.weight,
                petData.temperature,
                petData.humidity);
            pet->setActivityLevel(petData.activityLevel);
            return pet;
        }

        return nullptr;
    }
}

EventProcessor::EventProcessor(
    std::shared_ptr<ShelterManager> manager,
    std::shared_ptr<VetInspector> vetInspector) : manager(std::move(manager)), vetInspector(std::move(vetInspector)) {}

bool EventProcessor::processEvent(const std::shared_ptr<SimulationEvent> &event)
{
    lastError.clear();

    if (!event)
    {
        lastError = "Received a null simulation event.";
        return false;
    }

    switch (event->getType())
    {
    case EventType::ANIMAL_ARRIVED:
        return handleAnimalArrived(event);
    case EventType::ANIMAL_ADOPTED:
        return handleAnimalAdopted(event);
    case EventType::VACCINATION:
        return handleVaccination(event);
    case EventType::VET_CHECK:
        return handleVetCheck(event);
    default:
        lastError = "Unsupported event type: " + eventTypeToString(event->getType());
        return false;
    }
}

std::string EventProcessor::getLastError() const
{
    return lastError;
}

bool EventProcessor::handleAnimalArrived(const std::shared_ptr<SimulationEvent> &event)
{
    const auto arrivalEvent = std::dynamic_pointer_cast<AnimalArrivedEvent>(event);
    if (!arrivalEvent)
    {
        lastError = "Arrival event payload is invalid.";
        return false;
    }

    const short nextPetId = manager->generateNextPetId();
    auto newPet = createPetFromArrivalData(arrivalEvent->getPetData(), nextPetId);
    if (!newPet)
    {
        lastError = "Unable to create a pet from the arrival event.";
        return false;
    }

    if (!manager->addPet(std::move(newPet)))
    {
        lastError = "Unable to add the arriving pet to the shelter.";
        return false;
    }

    // Добавление заболеваний, сгенерированных для нового питомца
    for (const auto &issue : arrivalEvent->getHealthIssues())
    {
        manager->addHealthIssue(nextPetId, issue);
    }

    return true;
}

bool EventProcessor::handleAnimalAdopted(const std::shared_ptr<SimulationEvent> &event)
{
    const auto adoptionEvent = std::dynamic_pointer_cast<AnimalAdoptedEvent>(event);
    if (!adoptionEvent)
    {
        lastError = "Adoption event payload is invalid.";
        return false;
    }

    if (!manager->removePet(static_cast<short>(adoptionEvent->getPetId())))
    {
        lastError = "Unable to remove pet #" + std::to_string(adoptionEvent->getPetId()) + " during adoption.";
        return false;
    }

    return true;
}

bool EventProcessor::handleVaccination(const std::shared_ptr<SimulationEvent> &event)
{
    const auto vaccinationEvent = std::dynamic_pointer_cast<VaccinationEvent>(event);
    if (!vaccinationEvent)
    {
        lastError = "Vaccination event payload is invalid.";
        return false;
    }

    Pet *pet = manager->getPetById(static_cast<short>(vaccinationEvent->getPetId()));
    if (!pet)
    {
        lastError = "Unable to find pet #" + std::to_string(vaccinationEvent->getPetId()) + " for vaccination.";
        return false;
    }

    manager->addVaccination(
        static_cast<short>(vaccinationEvent->getPetId()),
        vaccinationEvent->getVaccineName(),
        vaccinationEvent->getDate());
    pet->changeHealth(5);
    return true;
}

bool EventProcessor::handleVetCheck(const std::shared_ptr<SimulationEvent> &event)
{
    const auto vetCheckEvent = std::dynamic_pointer_cast<VetCheckEvent>(event);
    if (!vetCheckEvent)
    {
        lastError = "Vet check event payload is invalid.";
        return false;
    }

    Pet *pet = manager->getPetById(static_cast<short>(vetCheckEvent->getPetId()));
    if (!pet)
    {
        lastError = "Unable to find pet #" + std::to_string(vetCheckEvent->getPetId()) + " for vet inspection.";
        return false;
    }

    const std::string report = vetInspector->performHealthCheck(*pet, manager->getMedicalRecord());
    manager->getLogger().info("VET", report);

    if (!vetInspector->isVaccinationUpToDate(*pet, manager->getMedicalRecord()))
    {
        manager->getLogger().warning("VET", pet->getName() + " needs vaccination follow-up.");
    }

    return true;
}
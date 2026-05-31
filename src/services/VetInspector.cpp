#include "../../include/shelter/services/VetInspector.h"
#include "../../include/shelter/core/MedicalRecord.h"
#include "../../include/shelter/core/Pet.h"
#include <iomanip>
#include <sstream>

namespace {
std::time_t parseDate(const std::string& dateString) {
    std::tm timeInfo = {};
    std::istringstream stream(dateString);
    stream >> std::get_time(&timeInfo, "%Y-%m-%d");
    if (stream.fail()) {
        return 0;
    }
    return std::mktime(&timeInfo);
}
}

bool VetInspector::isVaccinationUpToDate(const Pet& pet, const MedicalRecord& medicalRecord) const {
    const auto records = medicalRecord.getRecordsForPet(pet.getId());
    if (records.empty()) {
        return false;
    }

    for (const auto& record : records) {
        if (record.isActive && !isVaccinationExpired(record.vaccineName, parseDate(record.date))) {
            return true;
        }
    }
    return false;
}

std::vector<int> VetInspector::getAnimalsNeedingVaccination(
    const std::vector<Pet*>& animals,
    const MedicalRecord& medicalRecord
) const {
    std::vector<int> animalsNeedingVaccination;
    for (const Pet* pet : animals) {
        if (pet != nullptr && !isVaccinationUpToDate(*pet, medicalRecord)) {
            animalsNeedingVaccination.push_back(pet->getId());
        }
    }
    return animalsNeedingVaccination;
}

std::string VetInspector::performHealthCheck(const Pet& pet, const MedicalRecord& medicalRecord) const {
    std::ostringstream stream;
    stream << "Vet report for " << pet.getName()
           << ": health " << pet.getHealthLevel() << "/100, "
           << assessHungerStatus(pet) << ", "
           << (isVaccinationUpToDate(pet, medicalRecord) ? "vaccination current" : "vaccination overdue");

    if (pet.getHealthLevel() < 40) {
        stream << ", urgent follow-up recommended";
    } else if (pet.getHealthLevel() < 70) {
        stream << ", monitor closely";
    } else {
        stream << ", stable";
    }

    return stream.str();
}

std::string VetInspector::assessHungerStatus(const Pet& pet) const {
    const int hungerLevel = pet.getHungerLevel();
    if (hungerLevel >= HUNGER_CRITICAL_THRESHOLD) {
        return "critical hunger";
    }
    if (hungerLevel >= 50) {
        return "hungry";
    }
    if (hungerLevel >= 25) {
        return "needs a meal soon";
    }
    return "well fed";
}

bool VetInspector::isVaccinationExpired(const std::string& vaccineName, std::time_t vaccineDate) const {
    (void)vaccineName;
    if (vaccineDate == 0) {
        return true;
    }

    const std::time_t now = std::time(nullptr);
    const double ageInDays = std::difftime(now, vaccineDate) / (24.0 * 60.0 * 60.0);
    return ageInDays > static_cast<double>(VACCINATION_VALID_DAYS);
}

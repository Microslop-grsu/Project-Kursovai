#pragma once
#include <ctime>
#include <string>
#include <vector>

class MedicalRecord;
class Pet;

class VetInspector {
public:
    bool isVaccinationUpToDate(const Pet& pet, const MedicalRecord& medicalRecord) const;

    std::vector<int> getAnimalsNeedingVaccination(
        const std::vector<Pet*>& animals,
        const MedicalRecord& medicalRecord
    ) const;

    std::string performHealthCheck(const Pet& pet, const MedicalRecord& medicalRecord) const;
    std::string assessHungerStatus(const Pet& pet) const;

private:
    static constexpr int VACCINATION_VALID_DAYS = 365;
    static constexpr int HUNGER_CRITICAL_THRESHOLD = 80;

    bool isVaccinationExpired(const std::string& vaccineName, std::time_t vaccineDate) const;
};

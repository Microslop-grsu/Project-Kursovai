#pragma once
#include <string>

class Pet;
class MedicalRecord;

class DietCalculator {
public:
    static double calculateDailyGrams(const Pet &pet, const MedicalRecord &record);

    static std::string getDietSummary(const Pet &pet, const MedicalRecord &record);

private:
    static double baseGramsPerKg(const std::string &petType);
    static double activityMultiplier(const Pet &pet);
    static double healthAdjustment(short petId, const MedicalRecord &record);
};

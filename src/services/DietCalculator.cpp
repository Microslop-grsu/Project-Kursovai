#include "../../include/shelter/services/DietCalculator.h"
#include "../../include/shelter/core/Pet.h"
#include "../../include/shelter/core/MedicalRecord.h"
#include <sstream>
#include <iomanip>

double DietCalculator::baseGramsPerKg(const std::string &petType)
{
    if (petType == "Dog")  return 25.0;
    if (petType == "Cat")  return 40.0;
    if (petType == "Bird") return 15.0;
    return 20.0;
}

double DietCalculator::activityMultiplier(const Pet &pet)
{
    switch (pet.getActivityLevel()) {
        case ActivityLevel::LOW:  return 0.80;
        case ActivityLevel::HIGH: return 1.20;
        default:                  return 1.00;
    }
}

double DietCalculator::healthAdjustment(short petId, const MedicalRecord &record)
{
    double adj = 0.0;
    if (record.hasHealthIssue(petId, "obesity"))         adj -= 0.15;
    if (record.hasHealthIssue(petId, "diabetes"))        adj -= 0.10;
    if (record.hasHealthIssue(petId, "kidney_disease"))  adj -= 0.20;
    if (record.hasHealthIssue(petId, "underweight"))     adj += 0.20;
    return adj;
}

double DietCalculator::calculateDailyGrams(const Pet &pet, const MedicalRecord &record)
{
    double base      = baseGramsPerKg(pet.getType()) * pet.getWeight();
    double totalMult = activityMultiplier(pet) + healthAdjustment(pet.getId(), record);

    if (totalMult < 0.30) totalMult = 0.30;

    double result = base * totalMult;
    return result < 15.0 ? 15.0 : result;
}

std::string DietCalculator::getDietSummary(const Pet &pet, const MedicalRecord &record)
{
    double base      = baseGramsPerKg(pet.getType()) * pet.getWeight();
    double activity  = activityMultiplier(pet);
    double healthAdj = healthAdjustment(pet.getId(), record);
    double totalMult = activity + healthAdj;
    if (totalMult < 0.30) totalMult = 0.30;
    double grams = base * totalMult;
    if (grams < 5.0) grams = 5.0;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "--- Расчёт суточной нормы ---\n";
    oss << "  Вид           : " << pet.getType() << "\n";
    oss << "  Вес           : " << pet.getWeight() << " кг\n";
    oss << "  Базовая норма : " << baseGramsPerKg(pet.getType())
        << " г/кг × " << pet.getWeight() << " кг = " << base << " г\n";
    oss << "  Активность    : " << Pet::activityToString(pet.getActivityLevel())
        << " (×" << activity << ")\n";

    if (healthAdj != 0.0) {
        oss << "  Поправка (болезни): ";
        const auto issues = record.getHealthIssuesForPet(pet.getId());
        for (size_t i = 0; i < issues.size(); ++i) {
            if (i) oss << ", ";
            oss << issues[i];
        }
        oss << " → " << (healthAdj >= 0 ? "+" : "") << (healthAdj * 100) << "%\n";
    }

    oss << "  Итог          : " << grams << " г/сутки\n";
    return oss.str();
}

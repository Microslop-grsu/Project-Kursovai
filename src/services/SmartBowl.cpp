#include "../../include/shelter/services/SmartBowl.h"
#include "../../include/shelter/services/DietCalculator.h"
#include "../../include/shelter/services/FeedingMonitor.h"
#include "../../include/shelter/utils/Logger.h"
#include "../../include/shelter/core/Pet.h"
#include "../../include/shelter/core/MedicalRecord.h"
#include <string>
#include <vector>

SmartBowl::SmartBowl(short petId, double dailyLimitGrams)
    : petId(petId), dailyLimit(dailyLimitGrams) {}

bool SmartBowl::dispense(double grams, FeedingMonitor& monitor, Logger& logger)
{
    if (gramsConsumedToday + grams > dailyLimit) {
        logger.warning("BOWL",
            "Pet #" + std::to_string(petId) +
            ": daily limit " + std::to_string(static_cast<int>(dailyLimit)) +
            " g reached (consumed " + std::to_string(static_cast<int>(gramsConsumedToday)) +
            " g, requested " + std::to_string(static_cast<int>(grams)) + " g). Feeding blocked.");
        return false;
    }

    gramsConsumedToday += grams;
    monitor.feed(petId, grams);
    return true;
}

void SmartBowl::resetDay()                { gramsConsumedToday = 0.0; }
double SmartBowl::getConsumedToday() const { return gramsConsumedToday; }
double SmartBowl::getDailyLimit()    const { return dailyLimit; }

void SmartBowlRegistry::sync(const std::vector<Pet*>& pets, const MedicalRecord& record)
{
    for (Pet* pet : pets) {
        if (!pet) continue;
        short id = pet->getId();
        if (bowls.find(id) == bowls.end()) {
            double limit = DietCalculator::calculateDailyGrams(*pet, record);
            bowls.emplace(id, SmartBowl(id, limit));
        }
    }

    std::vector<short> toRemove;
    for (auto& [id, _] : bowls) {
        bool found = false;
        for (Pet* pet : pets) {
            if (pet && pet->getId() == id) { found = true; break; }
        }
        if (!found) toRemove.push_back(id);
    }
    for (short id : toRemove) bowls.erase(id);
}

bool SmartBowlRegistry::dispense(short petId, double grams, FeedingMonitor& monitor, Logger& logger)
{
    auto it = bowls.find(petId);
    if (it == bowls.end()) {
        monitor.feed(petId, grams);
        return true;
    }
    return it->second.dispense(grams, monitor, logger);
}

void SmartBowlRegistry::resetAll()
{
    for (auto& [id, bowl] : bowls)
        bowl.resetDay();
}
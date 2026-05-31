#include "../../include/shelter/services/FeedingMonitor.h"
#include "../../include/shelter/services/DietCalculator.h"
#include "../../include/shelter/utils/Logger.h"
#include "../../include/shelter/core/Pet.h"
#include "../../include/shelter/core/MedicalRecord.h"
#include <ctime>
#include <string>

namespace {
std::string petLabel(const Pet& pet) {
    return pet.getName() + " (#" + std::to_string(pet.getId()) + ")";
}
}

void FeedingMonitor::feed(short petId, double gramsGiven)
{
    std::time_t now = std::time(nullptr);
    lastMeal[petId] = now;

    auto it = dayStart.find(petId);
    if (it == dayStart.end()) {
        dayStart[petId] = now;
        dailyConsumed[petId] = 0.0;
    } else {
        if (std::difftime(now, it->second) >= 86400.0) {
            dailyConsumed[petId] = 0.0;
            dayStart[petId] = now;
        }
    }
    dailyConsumed[petId] += gramsGiven;
}

void FeedingMonitor::checkStarvation(Logger &logger, const std::vector<Pet *> &pets)
{
    std::time_t now = std::time(nullptr);
    const int thresholdSeconds = HUNGER_THRESHOLD_HOURS * 3600;

    for (Pet *pet : pets) {
        if (!pet) continue;
        const short petId = pet->getId();
        const std::string label = petLabel(*pet);

        auto it = lastMeal.find(petId);
        if (it == lastMeal.end()) {
            logger.warning("FEEDING", "Animal " + label + " has never been fed!");
            continue;
        }
        if (std::difftime(now, it->second) >= thresholdSeconds) {
            std::string msg = "Animal " + label + " has not been fed for over " +
                              std::to_string(HUNGER_THRESHOLD_HOURS) +
                              " hours! Last feeding: " + std::ctime(&it->second);
            if (!msg.empty() && msg.back() == '\n') msg.pop_back();
            logger.warning("FEEDING", msg);
        }
    }
}

void FeedingMonitor::checkOverfeeding(Logger &logger, const std::vector<Pet *> &pets,
                                       const MedicalRecord &record)
{
    for (Pet *pet : pets) {
        if (!pet) continue;
        const short petId = pet->getId();
        const std::string label = petLabel(*pet);

        auto it = dailyConsumed.find(petId);
        if (it == dailyConsumed.end()) continue; // ни разу не кормили сегодня

        double consumed = it->second;
        double norm     = DietCalculator::calculateDailyGrams(*pet, record);

        if (consumed > norm * 1.20) {  // превышение более чем на 20%
            int over = static_cast<int>((consumed / norm - 1.0) * 100);
            std::string msg = "Animal " + label + " is overfed! "
                + "Consumed: " + std::to_string(static_cast<int>(consumed)) + " g, "
                + "Daily norm: " + std::to_string(static_cast<int>(norm)) + " g "
                + "(" + std::to_string(over) + "% over)";
            logger.warning("DIET", msg);
        }
    }
}

void FeedingMonitor::resetDailyCounters() {
    dailyConsumed.clear();
    dayStart.clear();
}

std::time_t FeedingMonitor::getLastFeedingTime(short petId) const {
    auto it = lastMeal.find(petId);
    return (it != lastMeal.end()) ? it->second : 0;
}

double FeedingMonitor::getDailyConsumed(short petId) const {
    auto it = dailyConsumed.find(petId);
    return (it != dailyConsumed.end()) ? it->second : 0.0;
}

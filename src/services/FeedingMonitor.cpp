#include "../../include/shelter/services/FeedingMonitor.h"
#include "../../include/shelter/services/DietCalculator.h"
#include "../../include/shelter/utils/Logger.h"
#include "../../include/shelter/core/Pet.h"
#include "../../include/shelter/core/MedicalRecord.h"
#include <ctime>

void FeedingMonitor::feed(const std::string &petName, double gramsGiven)
{
    std::time_t now = std::time(nullptr);
    lastMeal[petName] = now;

    auto it = dayStart.find(petName);
    if (it == dayStart.end()) {
        dayStart[petName]      = now;
        dailyConsumed[petName] = 0.0;
    } else {
        if (std::difftime(now, it->second) >= 86400.0) {
            dailyConsumed[petName] = 0.0;
            dayStart[petName]      = now;
        }
    }
    dailyConsumed[petName] += gramsGiven;
}

void FeedingMonitor::checkStarvation(Logger &logger, const std::vector<Pet *> &pets)
{
    std::time_t now = std::time(nullptr);
    const int thresholdSeconds = HUNGER_THRESHOLD_HOURS * 3600;

    for (Pet *pet : pets) {
        if (!pet) continue;
        std::string name = pet->getName();

        auto it = lastMeal.find(name);
        if (it == lastMeal.end()) {
            logger.warning("FEEDING", "Animal " + name + " has never been fed!");
            continue;
        }
        if (std::difftime(now, it->second) >= thresholdSeconds) {
            std::string msg = "Animal " + name + " has not been fed for over " +
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
        std::string name = pet->getName();

        auto it = dailyConsumed.find(name);
        if (it == dailyConsumed.end()) continue; // ни разу не кормили сегодня

        double consumed = it->second;
        double norm     = DietCalculator::calculateDailyGrams(*pet, record);

        if (consumed > norm * 1.20) {  // превышение более чем на 20%
            int over = static_cast<int>((consumed / norm - 1.0) * 100);
            std::string msg = "Animal " + name + " is overfed! "
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

std::time_t FeedingMonitor::getLastFeedingTime(const std::string &petName) const {
    auto it = lastMeal.find(petName);
    return (it != lastMeal.end()) ? it->second : 0;
}

double FeedingMonitor::getDailyConsumed(const std::string &petName) const {
    auto it = dailyConsumed.find(petName);
    return (it != dailyConsumed.end()) ? it->second : 0.0;
}
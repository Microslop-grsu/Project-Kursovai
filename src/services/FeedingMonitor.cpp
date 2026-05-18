#include "../../include/shelter/services/FeedingMonitor.h"
#include "../../include/shelter/utils/Logger.h"
#include "../../include/shelter/core/Pet.h"
#include <vector> // добавлено
#include <ctime>

void FeedingMonitor::feed(const std::string &petName)
{
    lastMeal[petName] = std::time(nullptr);
}

void FeedingMonitor::checkStarvation(Logger &logger, const std::vector<Pet *> &pets)
{
    std::time_t now = std::time(nullptr);
    const int thresholdSeconds = HUNGER_THRESHOLD_HOURS * 3600;

    for (Pet *pet : pets)
    {
        if (!pet)
            continue;
        std::string name = pet->getName();

        auto it = lastMeal.find(name);
        if (it == lastMeal.end())
        {
            logger.warning("FEEDING", "Animal " + name + " has never been fed!");
            continue;
        }

        double diffSeconds = std::difftime(now, it->second);
        if (diffSeconds >= thresholdSeconds)
        {
            std::string msg = "Animal " + name + " has not been fed for over " +
                              std::to_string(HUNGER_THRESHOLD_HOURS) +
                              " hours! Last feeding: " + std::ctime(&it->second);
            // убираем символ новой строки
            if (!msg.empty() && msg.back() == '\n')
                msg.pop_back();
            logger.warning("FEEDING", msg);
        }
    }
}

std::time_t FeedingMonitor::getLastFeedingTime(const std::string &petName) const
{
    auto it = lastMeal.find(petName);
    return (it != lastMeal.end()) ? it->second : 0;
}

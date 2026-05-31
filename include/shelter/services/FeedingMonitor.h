#pragma once
#include <unordered_map>
#include <string>
#include <ctime>
#include <vector>

class Logger;
class Pet;
class MedicalRecord;

class FeedingMonitor {
public:
    // gramsGiven = 0 означает «покормили, но граммы не фиксируем»
    void feed(short petId, double gramsGiven = 0.0);

    void checkStarvation(Logger &logger, const std::vector<Pet *> &pets);

    // Сравнивает dailyConsumed с нормой DietCalculator, логирует превышение > 20%
    void checkOverfeeding(Logger &logger, const std::vector<Pet *> &pets,
                          const MedicalRecord &record);

    std::time_t getLastFeedingTime(short petId) const;
    double getDailyConsumed(short petId) const;
    void resetDailyCounters();

private:
    std::unordered_map<short, std::time_t> lastMeal;
    std::unordered_map<short, double> dailyConsumed; // grams consumed today
    std::unordered_map<short, std::time_t> dayStart; // current day tracking

    static const int HUNGER_THRESHOLD_HOURS = 8;
};

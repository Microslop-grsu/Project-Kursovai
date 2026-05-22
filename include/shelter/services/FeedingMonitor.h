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
    void feed(const std::string &petName, double gramsGiven = 0.0);

    void checkStarvation(Logger &logger, const std::vector<Pet *> &pets);

    // Сравнивает dailyConsumed с нормой DietCalculator, логирует превышение > 20%
    void checkOverfeeding(Logger &logger, const std::vector<Pet *> &pets,
                          const MedicalRecord &record);

    std::time_t getLastFeedingTime(const std::string &petName) const;
    double getDailyConsumed(const std::string &petName) const;
    void resetDailyCounters();

private:
    std::unordered_map<std::string, std::time_t> lastMeal;
    std::unordered_map<std::string, double>       dailyConsumed; // граммы за текущие сутки
    std::unordered_map<std::string, std::time_t>  dayStart;      // начало отсчёта суток

    static const int HUNGER_THRESHOLD_HOURS = 8;
};
#pragma once

#include <unordered_map>
#include <string>
#include <ctime>
#include <vector> // добавлено

class Logger;
class Pet; // forward declaration

class FeedingMonitor
{
public:
    void feed(const std::string &petName);
    void checkStarvation(Logger &logger, const std::vector<Pet *> &pets);
    std::time_t getLastFeedingTime(const std::string &petName) const;

private:
    std::unordered_map<std::string, std::time_t> lastMeal;
    static const int HUNGER_THRESHOLD_HOURS = 8;
};
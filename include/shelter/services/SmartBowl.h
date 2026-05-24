#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class Logger;
class Pet;
class MedicalRecord;
class FeedingMonitor;

class SmartBowl {
public:
    SmartBowl(short petId, double dailyLimitGrams);

    bool dispense(double grams, FeedingMonitor& monitor, Logger& logger);

    void resetDay();
    double getConsumedToday() const;
    double getDailyLimit() const;

private:
    short  petId;
    double dailyLimit;
    double gramsConsumedToday = 0.0;
};

class SmartBowlRegistry {
public:
    void sync(const std::vector<Pet*>& pets, const MedicalRecord& record);
    bool dispense(short petId, double grams, FeedingMonitor& monitor, Logger& logger);
    void resetAll();

private:
    std::unordered_map<short, SmartBowl> bowls;
};
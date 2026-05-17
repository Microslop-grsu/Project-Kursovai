#include "../../include/shelter/core/MedicalRecord.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Преобразование строки "YYYY-MM-DD" в time_t
std::time_t parseDateToTimeT(const std::string &dateStr)
{
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (ss.fail())
    {
        return 0; // Неверная дата считается просроченной
    }
    return std::mktime(&tm);
}

// Проверка, просрочена ли дата
bool MedicalRecord::isDateExpired(const std::string &dateStr)
{
    std::time_t now = std::time(nullptr);
    std::time_t vaccineDate = parseDateToTimeT(dateStr);
    return vaccineDate < now;
}

void MedicalRecord::addVaccination(short petId, const std::string &vaccineName, const std::string &date, bool isActive)
{
    records.emplace_back(petId, vaccineName, date, isActive);
}

bool MedicalRecord::hasExpired() const
{
    for (const auto &record : records)
    {
        if (!record.isActive)
            return true;
        if (isDateExpired(record.date))
            return true;
    }
    return false;
}

std::vector<VaccinationEntry> MedicalRecord::getExpiredVaccinations() const
{
    std::vector<VaccinationEntry> expired;
    for (const auto &record : records)
    {
        if (!record.isActive)
        {
            expired.push_back(record);
        }
        else if (isDateExpired(record.date))
        {
            VaccinationEntry expiredCopy = record;
            expiredCopy.isActive = false;
            expired.push_back(expiredCopy);
        }
    }
    return expired;
}

void MedicalRecord::updateStatuses()
{
    for (auto &record : records)
    {
        if (record.isActive && isDateExpired(record.date))
        {
            record.isActive = false;
        }
    }
}

const std::vector<VaccinationEntry> &MedicalRecord::getAllRecords() const
{
    return records;
}

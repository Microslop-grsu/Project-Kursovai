#include "../../include/shelter/core/MedicalRecord.h"
#include <ctime>
#include <iomanip>
#include <sstream>

namespace {
constexpr double kVaccinationValiditySeconds = 365.0 * 24.0 * 60.0 * 60.0;

std::time_t parseDateToTimeT(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (ss.fail()) {
        return 0;
    }
    return std::mktime(&tm);
}
}

bool MedicalRecord::isDateExpired(const std::string& dateStr) {
    const std::time_t now = std::time(nullptr);
    const std::time_t vaccineDate = parseDateToTimeT(dateStr);
    if (vaccineDate == 0) {
        return true;
    }
    return std::difftime(now, vaccineDate) > kVaccinationValiditySeconds;
}

void MedicalRecord::addVaccination(short petId, const std::string& vaccineName, const std::string& date, bool isActive) {
    records.emplace_back(petId, vaccineName, date, isActive);
}

bool MedicalRecord::hasExpired() const {
    for (const auto& record : records) {
        if (!record.isActive || isDateExpired(record.date)) {
            return true;
        }
    }
    return false;
}

std::vector<VaccinationEntry> MedicalRecord::getExpiredVaccinations() const {
    std::vector<VaccinationEntry> expired;
    for (const auto& record : records) {
        if (!record.isActive) {
            expired.push_back(record);
            continue;
        }

        if (isDateExpired(record.date)) {
            VaccinationEntry expiredCopy = record;
            expiredCopy.isActive = false;
            expired.push_back(expiredCopy);
        }
    }
    return expired;
}

std::vector<VaccinationEntry> MedicalRecord::getRecordsForPet(short petId) const {
    std::vector<VaccinationEntry> petRecords;
    for (const auto& record : records) {
        if (record.petId == petId) {
            petRecords.push_back(record);
        }
    }
    return petRecords;
}

bool MedicalRecord::hasVaccinationForPet(short petId) const {
    for (const auto& record : records) {
        if (record.petId == petId) {
            return true;
        }
    }
    return false;
}

void MedicalRecord::updateStatuses() {
    for (auto& record : records) {
        if (record.isActive && isDateExpired(record.date)) {
            record.isActive = false;
        }
    }
}

const std::vector<VaccinationEntry>& MedicalRecord::getAllRecords() const {
    return records;
}

void MedicalRecord::addHealthIssue(short petId, const std::string& issue) {
    for (const auto& entry : healthIssues) {
        if (entry.petId == petId && entry.issue == issue) {
            return;  // уже добавлен
        }
    }
    healthIssues.emplace_back(petId, issue);
}

bool MedicalRecord::hasHealthIssue(short petId, const std::string& issue) const {
    for (const auto& entry : healthIssues) {
        if (entry.petId == petId && entry.issue == issue) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> MedicalRecord::getHealthIssuesForPet(short petId) const {
    std::vector<std::string> issues;
    for (const auto& entry : healthIssues) {
        if (entry.petId == petId) {
            issues.push_back(entry.issue);
        }
    }
    return issues;
}

const std::vector<HealthIssueEntry>& MedicalRecord::getAllHealthIssues() const {
    return healthIssues;
}

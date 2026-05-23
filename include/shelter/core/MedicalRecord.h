#pragma once
#include <string>
#include <vector>
#include <ctime>

struct VaccinationEntry {
    short petId;
    std::string vaccineName;
    std::string date;
    bool isActive;

    VaccinationEntry(short id, const std::string &vaccine,
                     const std::string &dateStr, bool active = true)
        : petId(id), vaccineName(vaccine), date(dateStr), isActive(active) {}
};

class MedicalRecord {
public:
    void addVaccination(short petId, const std::string &vaccineName,
                        const std::string &date, bool isActive = true);
    bool hasExpired() const;
    std::vector<VaccinationEntry> getExpiredVaccinations() const;
    std::vector<VaccinationEntry> getRecordsForPet(short petId) const;
    bool hasVaccinationForPet(short petId) const;
    void updateStatuses();
    const std::vector<VaccinationEntry> &getAllRecords() const;

    // --- Диагнозы для расчёта диеты ---
    void addHealthIssue(const std::string &issue);          // добавить диагноз
    bool hasHealthIssue(const std::string &issue) const;    // проверить наличие
    const std::vector<std::string> &getHealthIssues() const; // список всех

private:
    std::vector<VaccinationEntry> records;
    std::vector<std::string> healthIssues;

    static bool isDateExpired(const std::string &dateStr);
};

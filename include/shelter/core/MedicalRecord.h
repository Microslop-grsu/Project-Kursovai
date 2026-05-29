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

struct HealthIssueEntry {
    short petId;
    std::string issue;

    HealthIssueEntry(short id, const std::string &healthIssue)
        : petId(id), issue(healthIssue) {}
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

    // --- Диагнозы для расчёта диеты (привязаны к petId) ---
    void addHealthIssue(short petId, const std::string &issue);          // добавить диагноз для питомца
    bool hasHealthIssue(short petId, const std::string &issue) const;    // проверить наличие диагноза у питомца
    std::vector<std::string> getHealthIssuesForPet(short petId) const;   // список диагнозов для питомца
    const std::vector<HealthIssueEntry> &getAllHealthIssues() const;     // все диагнозы всех питомцев

private:
    std::vector<VaccinationEntry> records;
    std::vector<HealthIssueEntry> healthIssues;

    static bool isDateExpired(const std::string &dateStr);
};

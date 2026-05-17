#pragma once

#include <string>
#include <vector>
#include <ctime>

// Структура для хранения одной записи о прививке
struct VaccinationEntry
{
    short petId;
    std::string vaccineName;
    std::string date; // Формат: YYYY-MM-DD
    bool isActive;

    VaccinationEntry(short id, const std::string &vaccine, const std::string &dateStr, bool active = true)
        : petId(id), vaccineName(vaccine), date(dateStr), isActive(active) {}
};

class MedicalRecord
{
public:
    // Добавить новую прививку
    void addVaccination(short petId, const std::string &vaccineName, const std::string &date, bool isActive = true);

    // Проверить, есть ли просроченные прививки
    bool hasExpired() const;

    // Получить список всех просроченных прививок
    std::vector<VaccinationEntry> getExpiredVaccinations() const;

    // Обновить статусы всех прививок на основе текущей даты
    void updateStatuses();

    // Получить список всех прививок
    const std::vector<VaccinationEntry> &getAllRecords() const;

private:
    std::vector<VaccinationEntry> records;

    // Вспомогательная функция для сравнения дат
    static bool isDateExpired(const std::string &dateStr);
};

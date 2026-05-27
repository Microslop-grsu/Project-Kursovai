#include "../../include/shelter/utils/Logger.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>

Logger::Logger(const std::string& file_name) {
    log_file.open(file_name, std::ios::app);
    if (!log_file.is_open()) {
        std::cerr << "Failed to open log file: " << file_name << std::endl;
    }
}

Logger::~Logger() {
    if (log_file.is_open())
        log_file.close();
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now{};

#if defined(_WIN32)
    localtime_s(&tm_now, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_now);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void Logger::log(LogLevel level, const std::string& category, const std::string& message) {
    if (log_file.is_open()) {
        log_file << getCurrentTime() << " [" << levelToString(level) << "] "
               << category << " - " << message << std::endl;
    }
}

void Logger::info(const std::string& category, const std::string& message) {
    log(LogLevel::INFO, category, message);
}
void Logger::warning(const std::string& category, const std::string& message) {
    log(LogLevel::WARNING, category, message);
}
void Logger::error(const std::string& category, const std::string& message) {
    log(LogLevel::ERROR, category, message);
}
void Logger::debug(const std::string& category, const std::string& message) {
    log(LogLevel::DEBUG, category, message);
}


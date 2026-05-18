#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

class Logger {
public:
    explicit Logger(const std::string& fileName);

    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void log(LogLevel level, const std::string& category, const std::string& message);

    void info(const std::string& category, const std::string& message);
    void warning(const std::string& category, const std::string& message);
    void error(const std::string& category, const std::string& message);
    void debug(const std::string& category, const std::string& message);

private:
    std::ofstream log_file;

    std::string levelToString(LogLevel level);
    std::string getCurrentTime();
};


#endif //LOGGER_H

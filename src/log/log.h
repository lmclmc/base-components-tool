#ifndef LOG_H_
#define LOG_H_

#include <iostream>

using std::string;

#define LOGGER_INFO     (lmc::Logger(LogLevel::info) << " : " << __LINE__ <<  " : " << __FUNCTION__ << " : ")
#define LOGGER_DEBUG     (lmc::Logger(LogLevel::debug) << " : " << __LINE__ <<  " : " << __FUNCTION__ << " : ")
#define LOGGER_ERROR     (lmc::Logger(LogLevel::error) << " : " << __LINE__ <<  " : " << __FUNCTION__ << " : ")
#define LOGGER_WARNING     (lmc::Logger(LogLevel::warning) << " : " << __LINE__ <<  " : " << __FUNCTION__ << " : ")
#define LOGGER  LOGGER_INFO

typedef enum class Level_: unsigned char
{
    reserve,
    nolog,
    info,
    warning,
    debug,
    error
}LogLevel;

namespace lmc
{
    class Logger
    {
    public:
        Logger(const LogLevel &level);
        ~Logger();

        Logger& operator << (const string &str);
        Logger& operator << (const char *src);
        Logger& operator << (uint64_t num);
        Logger& operator << (uint32_t num);
        Logger& operator << (int num);
        Logger& operator << (long int num);
        string getString();

    private:
        bool judgeLevel();

    private:
        static LogLevel gLevel;
        static string gLogLevelStr;
        LogLevel mLevel;
        string strLog;
    };
}

#endif
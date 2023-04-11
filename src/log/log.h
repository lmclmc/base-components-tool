#ifndef LOG_H_
#define LOG_H_

#include <iostream>
#include <elf.h>

using std::string;

#define LOGGER_INFO     (lmc::Logger(LogLevel::info) << " : " << __LINE__ <<  " : " << __FUNCTION__ << " : ")
#define LOGGER_DEBUG    (lmc::Logger(LogLevel::debug) << " : " << __LINE__ <<  " : " << __FUNCTION__ << " : ")
#define LOGGER_ERROR    (lmc::Logger(LogLevel::error) << " : " << __LINE__ <<  " : " << __FUNCTION__ << " : ")
#define LOGGER_WARNING  (lmc::Logger(LogLevel::warning) << " : " << __LINE__ <<  " : " << __FUNCTION__ << " : ")
#define LOGGER          (lmc::Logger(LogLevel::print))

typedef enum class LogLevel_: unsigned char
{
    close,
    print,
    info,
    warning,
    debug,
    error,
    all
}LogLevel;

typedef enum class LogFormat_: unsigned char
{
    num,
    addr
}LogFormat;

namespace lmc
{
class Logger
{
public:
    Logger(const LogLevel &level);
    ~Logger();

    Logger &operator << (const std::string &);
    Logger &operator << (const char *);
    Logger &operator << (uint64_t);
    Logger &operator << (uint32_t);
    Logger &operator << (double);
    Logger &operator << (int);
    Logger &operator << (long int);
    Logger &operator << (LogFormat);
    string getString();

    static void setOutputFile(const std::string &);
    static void setLevel(LogLevel);
    static LogLevel getLevel();
private:
    bool judgeLevel();

private:
    static LogLevel sLevel;
    LogLevel mLevel;
    std::string strLog;
    static std::string sOutputFile;
    static LogFormat sLogFormat;
    static int sOutputFd;
};
}

#endif
#ifndef LOG_H_
#define LOG_H_

#include <iostream>
#include <elf.h>

#include "util/type.hpp"

#define LOGGER_INFO     (lmc::Logger(LogLevel::info) << " : " << __LINE__ <<  " : " << __FUNCTION__ << " : ")
#define LOGGER_DEBUG    (lmc::Logger(LogLevel::debug) << " : " << __LINE__ <<  " : " << __FUNCTION__ << " : ")
#define LOGGER_ERROR    (lmc::Logger(LogLevel::error) << " : " << __LINE__ <<  " : " << __FUNCTION__ << " : ")
#define LOGGER_WARNING  (lmc::Logger(LogLevel::warning) << " : " << __LINE__ <<  " : " << __FUNCTION__ << " : ")
#define LOGGER          (lmc::Logger(LogLevel::print))

#define BUFFER_SIZE (128)

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
template<bool IsNum>
struct Trans
{
    template<typename T>
    std::string operator()(const T &value, LogFormat)
    {
        return value;
    }
};

template<>
struct Trans<true>
{
    template<typename T>
    std::string operator()(const T &value, LogFormat format)
    {
        if (format == LogFormat::addr)
        {
            char buffer[BUFFER_SIZE] = {0};
            snprintf(buffer, BUFFER_SIZE, "0x%lx", (long unsigned int)value);
            return buffer;
        }
        else
        {
            return std::to_string(value);
        }
    }
};

class Logger
{
public:
    Logger(const LogLevel &level);
    ~Logger();

    template<typename T>
    Logger &operator << (const T &value)
    {
        if (!judgeLevel()) return *this;

        strLog += Trans<Search<lmc::RemoveCVREF<T>, 
                               NumTypeList>::status>()(value, sLogFormat);
        return *this;
    }

    Logger &operator << (LogFormat);
    std::string getString();

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
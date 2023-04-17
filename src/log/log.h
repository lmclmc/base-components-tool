#ifndef LOG_H_
#define LOG_H_

#include <iostream>
#include <elf.h>

#include "type/type.h"

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
    std::string operator()(T &value)
    {
        return value;
    }
};

template<>
struct Trans<true>
{
    template<typename T>
    std::string operator()(T &value)
    {
        return std::to_string(value);
    }
};

class Logger
{
public:
    Logger(const LogLevel &level);
    ~Logger();

    template<typename T>
    Logger &operator << (T &value)
    {
        if (!judgeLevel()) return *this;

        strLog += Trans<Search<typename std::remove_cv<T>::type, 
                               NumTypeList>::status>()(value);
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
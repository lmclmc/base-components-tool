#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "log.h"

using namespace std;
using namespace lmc;

#define BUFFER_SIZE (128)

LogLevel lmc::Logger::sLevel = LogLevel::reserve;
std::string Logger::sOutputFile = "";
LogFormat Logger::sLogFormat = LogFormat::num;
int Logger::sOutputFd = 0;

Logger::Logger(const LogLevel &level) :
    mLevel(level)
{
    if (LogLevel::clear == level)
        return;
        
    std::string logLevelStr;
    if (LogLevel::reserve == sLevel)
    {
        sLevel = LogLevel::info;

        const char *sc = ::getenv("LOG_LEVEL");
        if (NULL == sc)
        {
            sLevel = LogLevel::info;
            logLevelStr = "info";
        } else 
        {
            string s = sc;
            if ("nolog" == s)
            {
                sLevel = LogLevel::nolog;
            }
            else if ("info" == s)
            {
                sLevel = LogLevel::info;
            } else if ("warning" == s)
            {
                sLevel = LogLevel::warning;
            } else if ("debug" == s)
            {
                sLevel = LogLevel::debug;
            } else if ("error" == s)
            {
                sLevel = LogLevel::error;
            }
            logLevelStr = s;
        }
    }

    if (!judgeLevel()) return;

    strLog = string("====   ");
    strLog += logLevelStr;
    strLog += "  ";
    struct timeval tv;
    struct tm *tm;
    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);
    char buffer[BUFFER_SIZE] = {0};
    snprintf(buffer, BUFFER_SIZE, "%d-%d-%d %d:%d:%d.%ld", tm->tm_year+1900, tm->tm_mon+1,
                                             tm->tm_mday, tm->tm_hour,
                                             tm->tm_min, tm->tm_sec, tv.tv_usec);
    strLog += buffer;
    strLog += "   ";
}

Logger::~Logger()
{
    if (!judgeLevel()) return;

    if (sOutputFile.empty())
        cout << strLog << endl;
    else if (!sOutputFd)
    {
        sOutputFd = open(sOutputFile.c_str(), O_CREAT | O_APPEND | O_RDWR, 0666);
        if (sOutputFd == -1)
            return;

        if (write(sOutputFd, strLog.c_str(), strLog.size()) == -1)
            cout << "write : error " << strerror(errno) << endl;
    }
    else
        if (write(sOutputFd, strLog.c_str(), strLog.size()) == -1)
            cout << "write : error " << strerror(errno) << endl;
}

Logger &Logger::operator << (const string& str)
{
    if (!judgeLevel()) return *this;

    strLog += str;
    return *this;
}

Logger &Logger::operator << (LogFormat logFormat)
{
    if (!judgeLevel()) return *this;
    sLogFormat = logFormat;
    return *this;
}

Logger &Logger::operator << (uint64_t num)
{
    if (!judgeLevel()) return *this;

    if (sLogFormat == LogFormat::addr)
    {
        char buffer[BUFFER_SIZE] = {0};
        void *p = (void *)num;
        snprintf(buffer, BUFFER_SIZE, "%p", p);
        strLog += buffer;
    }
    else
        strLog += std::to_string(num);

    return *this;
}

Logger& Logger::operator << (uint32_t num)
{
    if (!judgeLevel()) return *this;

    strLog += std::to_string(num);
    return *this;
}

Logger &Logger::operator << (int num)
{
    if (!judgeLevel()) return *this;

    strLog += std::to_string(num);
    return *this;
}

Logger &Logger::operator << (const char *src)
{
    if (!judgeLevel()) return *this;

    if (NULL == src) return *this;

    strLog += src;
    return *this;
}

Logger &Logger::operator << (long int num)
{
    if (!judgeLevel()) return *this;

    strLog += std::to_string(num);
    return *this;
}

bool Logger::judgeLevel()
{
    if (mLevel <= sLevel)
    {
        return true;
    }

    return false;
}

string Logger::getString()
{
    return strLog;
}

void Logger::setOutputFile(const std::string &file)
{
    sOutputFile = file;
}
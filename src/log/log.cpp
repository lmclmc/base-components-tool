#include <sys/time.h>

#include "log.h"

using namespace std;
using namespace lmc;

LogLevel lmc::Logger::gLevel = LogLevel::reserve;
string lmc::Logger::gLogLevelStr;

Logger::Logger(const LogLevel &level) :
    mLevel(level)
{
    if (LogLevel::reserve == gLevel)
    {
        gLevel = LogLevel::info;

        const char *sc = ::getenv("LOG_LEVEL");
        if (NULL == sc)
        {
            gLevel = LogLevel::info;
            gLogLevelStr = "info";
        } else 
        {
            string s = sc;
            if ("nolog" == s)
            {
                gLevel = LogLevel::nolog;
            }
            else if ("info" == s)
            {
                gLevel = LogLevel::info;
            } else if ("warning" == s)
            {
                gLevel = LogLevel::warning;
            } else if ("debug" == s)
            {
                gLevel = LogLevel::debug;
            } else if ("error" == s)
            {
                gLevel = LogLevel::error;
            }
            gLogLevelStr = s;
        }
    }

    if (!judgeLevel()) return;

    strLog = string("====   ");
    strLog += gLogLevelStr;
    strLog += "  ";
    struct timeval tv;
    struct tm *tm;
    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);
    char buffer[32] = {0};
    sprintf(buffer, "%d-%d-%d %d:%d:%d.%ld", tm->tm_year+1900, tm->tm_mon+1,
                                             tm->tm_mday, tm->tm_hour,
                                             tm->tm_min, tm->tm_sec, tv.tv_usec);
    strLog += buffer;
    strLog += "   ";
}

Logger::~Logger()
{
    if (!judgeLevel()) return;
    cout << strLog << endl;
}

Logger& Logger::operator << (const string& str)
{
    if (!judgeLevel()) return *this;

    strLog += str;
    return *this;
}

Logger& Logger::operator << (uint64_t num)
{
    if (!judgeLevel()) return *this;

    strLog += std::to_string(num);
    return *this;
}

Logger& Logger::operator << (uint32_t num)
{
    if (!judgeLevel()) return *this;

    strLog += std::to_string(num);
    return *this;
}

Logger& Logger::operator << (int num)
{
    if (!judgeLevel()) return *this;

    strLog += std::to_string(num);
    return *this;
}

Logger& Logger::operator << (const char *src)
{
    if (!judgeLevel()) return *this;

    if (NULL == src) return *this;

    strLog += src;
    return *this;
}

Logger& Logger::operator << (long int num)
{
    if (!judgeLevel()) return *this;

    strLog += std::to_string(num);
    return *this;
}

bool Logger::judgeLevel()
{
    if (mLevel <= gLevel)
    {
        return true;
    }

    return false;
}

string Logger::getString()
{
    return strLog;
}


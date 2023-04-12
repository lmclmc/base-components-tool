#include "cmdline/cmdline.h"
#include "single/single.hpp"
#include "log/log.h"

#include <vector>
#include <list>
using namespace lmc;

int main(int argc, char *argv[])
{
    CmdLine *cmd = TypeSingle<CmdLine>::getInstance();
    cmd->add<std::vector, short>("-s", "--short", "get short");
    cmd->add<std::vector, unsigned short>("-us", "--us", "get unsigned short");
    cmd->add<std::vector, int>("-i", "--int", "get int");
    cmd->add<std::vector, unsigned int>("-ui", "--ui", "get unsigned int");
    cmd->add<std::vector, float>("-f", "--float", "get float", {}, {23, 100});
    cmd->add<std::vector, double>("-d", "--double", "get double");
    cmd->add<std::vector, long>("-l", "--long", "get long");
    cmd->add<std::vector, unsigned long>("-ul", "--ul", "get unsigned long");
    cmd->add<std::vector, std::string>("-string", "--string", "get string");
    cmd->add<std::list, std::string>("-ls", "--lstring", "get list string");
    cmd->parse(argc, argv);

    Logger::setLevel(LogLevel::all);

    std::vector<short> sVector;
    bool ret = cmd->get<std::vector, short>("--short", sVector);
    if (ret)
    {
        for (auto &v : sVector)
        {
            LOGGER << v;
        }
    }

    std::vector<unsigned short> usVector;
    ret = cmd->get<std::vector, unsigned short>("--us", usVector);
    if (ret)
    {
        for (auto &v : usVector)
        {
            LOGGER << v;
        }
    }

    std::vector<unsigned int> uiVector;
    ret = cmd->get<std::vector, unsigned int>("--ui", uiVector);
    if (ret)
    {
        for (auto &v : uiVector)
        {
            LOGGER << v;
        }
    }

    std::vector<int> iVector;
    ret = cmd->get<std::vector, int>("--int", iVector);
    if (ret)
    {
        for (auto &v : iVector)
        {
            LOGGER << v;
        }
    }

    std::vector<float> fVector;
    ret = cmd->get<std::vector, float>("--float", fVector);
    if (ret)
    {
        for (auto &v : fVector)
        {
            LOGGER << v;
        }
    }

    std::vector<double> dVector;
    ret = cmd->get<std::vector, double>("--double", dVector);
    if (ret)
    {
        for (auto &v : dVector)
        {
            LOGGER << v;
        }
    }

    std::vector<long> lVector;
    ret = cmd->get<std::vector, long>("--long", lVector);
    if (ret)
    {
        for (auto &v : lVector)
        {
            LOGGER << v;
        }
    }

    std::vector<unsigned long> ulVector;
    ret = cmd->get<std::vector, unsigned long>("--ul", ulVector);
    if (ret)
    {
        for (auto &v : ulVector)
        {
            LOGGER << v;
        }
    }
 
    std::vector<std::string> strVector;
    ret = cmd->get<std::vector, std::string>("--string", strVector);
    if (ret)
    {
        for (auto &v : strVector)
        {
            LOGGER << v;
        }
    }

    std::list<std::string> strList;
    ret = cmd->get<std::list, std::string>("--lstring", strList);
    if (ret)
    {
        for (auto &v : strList)
        {
            LOGGER << v;
        }
    }
}
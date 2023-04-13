#include "cmdline/cmdline.h"
#include "single/single.hpp"
#include "log/log.h"

#include <vector>
#include <list>
#include <set>
#include <queue>
#include <stack>

using namespace lmc;

int main(int argc, char *argv[])
{
    CmdLine *cmd = TypeSingle<CmdLine>::getInstance();
    cmd->add<std::vector<short>>("-s", "--short", "get short", {}, {33, 55});
    cmd->add<std::vector<unsigned short>>("-us", "--us", "get unsigned short");
    cmd->add<std::vector<int>>("-i", "--int", "get int");
    cmd->add<std::vector<unsigned int>>("-ui", "--ui", "get unsigned int");
    cmd->add<std::vector<float>>("-f", "--float", "get float", {}, {23, 100});
    cmd->add<std::vector<double>>("-d", "--double", "get double");
    cmd->add<std::vector<long>>("-l", "--long", "get long");
    cmd->add<std::vector<unsigned long>>("-ul", "--ul", "get unsigned long");
    cmd->add<std::vector<std::string>>("-string", "--string", "get string");
    cmd->add<std::list<std::string>>("-ls", "--lstring", "get list string");
    cmd->add<std::set<std::string>>("-ss", "--sstring", "get set string");
    cmd->add<std::set<int>>("-si", "--sint", "get set int", {}, {44, 99});
    cmd->parse(argc, argv);

    Logger::setLevel(LogLevel::all);

    std::vector<short> sVector;
    bool ret = cmd->get("--short", sVector);
    if (ret)
    {
        for (auto &v : sVector)
        {
            LOGGER << v;
        }
    }

    std::vector<unsigned short> usVector;
    ret = cmd->get("--us", usVector);
    if (ret)
    {
        for (auto &v : usVector)
        {
            LOGGER << v;
        }
    }

    std::vector<unsigned int> uiVector;
    ret = cmd->get("--ui", uiVector);
    if (ret)
    {
        for (auto &v : uiVector)
        {
            LOGGER << v;
        }
    }

    std::vector<int> iVector;
    ret = cmd->get("--int", iVector);
    if (ret)
    {
        for (auto &v : iVector)
        {
            LOGGER << v;
        }
    }

    std::vector<float> fVector;
    ret = cmd->get("--float", fVector);
    if (ret)
    {
        for (auto &v : fVector)
        {
            LOGGER << v;
        }
    }

    std::vector<double> dVector;
    ret = cmd->get("--double", dVector);
    if (ret)
    {
        for (auto &v : dVector)
        {
            LOGGER << v;
        }
    }

    std::vector<long> lVector;
    ret = cmd->get("--long", lVector);
    if (ret)
    {
        for (auto &v : lVector)
        {
            LOGGER << v;
        }
    }

    std::vector<unsigned long> ulVector;
    ret = cmd->get("--ul", ulVector);
    if (ret)
    {
        for (auto &v : ulVector)
        {
            LOGGER << v;
        }
    }
 
    std::vector<std::string> strVector;
    ret = cmd->get("--string", strVector);
    if (ret)
    {
        for (auto &v : strVector)
        {
            LOGGER << v;
        }
    }

    std::list<std::string> strList;
    ret = cmd->get("--lstring", strList);
    if (ret)
    {
        for (auto &v : strList)
        {
            LOGGER << v;
        }
    }

    std::set<std::string> strSet;
    ret = cmd->get("--sstring", strSet);
    if (ret)
    {
        for (auto &v : strSet)
        {
            LOGGER << v;
        }
    }

    std::set<int> intSet;
    ret = cmd->get("--sint", intSet);
    if (ret)
    {
        for (auto &v : intSet)
        {
            LOGGER << v;
        }
    }
}
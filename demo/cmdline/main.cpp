#include "cmdline/cmdline.h"
#include "single/single.hpp"
#include "log/log.h"

#include <vector>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>

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
    cmd->add<std::vector<std::string>>("-string", "--string", "get string", 
                                       {}, {"aaa", "vvv", "bbb", "rrr", "ttt"});
    cmd->add<std::list<std::string>>("-ls", "--lstring", "get list string", 
                                     {}, {"aaa", "vvv", "bbb", "rrr", "ttt"});
    cmd->add<std::set<std::string>>("-ss", "--sstring", "get set string", 
                                    {}, {"aaa", "vvv", "bbb", "rrr", "ttt"});
    cmd->add<std::set<int>>("-si", "--sint", "get set int", {"-ss", "-us"}, {44, 99});
    cmd->add<std::deque<std::string>>("-ds", "--dstring", "get deque string", 
                                      {}, {"aaa", "vvv", "bbb", "rrr", "ttt"});
    cmd->add<std::deque<int>>("-di", "--dint", "get deque int", {}, {44, 99});
    cmd->add<std::queue<std::string>>("-dsss", "--queuestring", "get queue string", 
                                      {}, std::queue<std::string>({"aaa", "vvv", "bbb", "rrr", "ttt"}));
    cmd->add<std::queue<int>>("-diss", "--queueint", "get queue int", {}, std::queue<int>({44, 99}));
    cmd->add<std::stack<std::string>>("-dsa", "--stackstring", "get stack string", 
                                      {}, std::stack<std::string>({"aaa", "vvv", "bbb", "rrr", "ttt"}));
    cmd->add<std::stack<int>>("-dix", "--stackint", "get stack int", {}, std::stack<int>({44, 99}));
    // std::stack<int> a({23, 33, 44});
   
    // std::stack<int> b = a;
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

    std::deque<std::string> strDeque;
    ret = cmd->get("--dstring", strDeque);
    if (ret)
    {
        for (auto &v : strDeque)
        {
            LOGGER << v;
        }
    }

    std::deque<int> intDeque;
    ret = cmd->get("--dint", intDeque);
    if (ret)
    {
        for (auto &v : intDeque)
        {
            LOGGER << v;
        }
    }

    std::queue<std::string> strQueue;
    ret = cmd->get("--queuestring", strQueue);
    if (ret)
    {
        int size = strQueue.size();
        for (int i = 0; i < size; i++)
        {
            auto s = strQueue.front();
            LOGGER << s;
            strQueue.pop();
        }
    }

    std::queue<int> intQueue;
    ret = cmd->get("--queueint", intQueue);
    if (ret)
    {
        int size = intQueue.size();
        for (int i = 0; i < size; i++)
        {
            auto s = intQueue.front();
            LOGGER << s;
            intQueue.pop();
        }
    }

    std::stack<std::string> strStack;
    ret = cmd->get("--stackstring", strStack);
    if (ret)
    {
        int size = strStack.size();
        for (int i = 0; i < size; i++)
        {
            auto s = strStack.top();
            LOGGER << s;
            strStack.pop();
        }
    }

    std::stack<int> intStack;
    ret = cmd->get("--stackint", intStack);
    if (ret)
    {
        int size = intStack.size();
        for (int i = 0; i < size; i++)
        {
            auto s = intStack.top();
            LOGGER << s;
            intStack.pop();
        }
    }
}
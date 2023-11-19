#include "version.h"

#include "cmdline/cmdline.h"
#include "util/single.hpp"
#include "log/log.h"

#include <vector>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <forward_list>
#include <unordered_set>

using namespace lmc;

int main(int argc, char *argv[])
{
    CmdLine *cmd = TypeSingle<CmdLine>::getInstance();
    cmd->add<std::vector<short>>("-s", "--short", "get short", {}, {33, 55});
    cmd->add<std::vector<unsigned short>>("-us", "--us", "get unsigned short");
    cmd->add<std::vector<int>>("-i", "--int", "get int");
    cmd->add<std::vector<unsigned int>>("-ui", "--ui", "get unsigned int",
                                        {"-s", "-us", "-i"});
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
    cmd->add<std::set<int>>("-si", "--sint", "get set int", 
                           {"-ss", "-us"}, {44, 99});
    cmd->add<std::deque<std::string>>("-ds", "--dstring", "get deque string", 
                                      {}, {"aaa", "vvv", "bbb", "rrr", "ttt"});
    cmd->add<std::deque<int>>("-di", "--dint", "get deque int", {}, {44, 99});
#if __GNUC__ > 6
    cmd->add<std::queue<std::string>>("-dsss", "--queuestring", 
                                      "get queue string", 
                                      {}, std::queue<std::string>({"aaa", 
                                      "vvv", "bbb", "rrr", "ttt"}));
    cmd->add<std::queue<int>>("-diss", "--queueint", "get queue int", 
                              {}, std::queue<int>({44, 99}));
    cmd->add<std::stack<std::string>>("-dsa", "--stackstring", 
                                      "get stack string", 
                                      {}, std::stack<std::string>({"aaa", 
                                      "vvv", "bbb", "rrr", "ttt"}));
    cmd->add<std::stack<int>>("-dix", "--stackint", "get stack int", 
                              {}, std::stack<int>({44, 99}));
    cmd->add<std::forward_list<std::string>>("-flstring", 
                                             "--forwardliststring", 
                                             "get forward_list string", 
                                             {}, {"aaa", "vvv", "bbb", 
                                             "rrr", "ttt"});
    cmd->add<std::forward_list<int>>("-flint", "--forwardlistint", 
                                     "get forward_list int", {}, {44, 99});
    cmd->add<std::unordered_set<std::string>>("-unstr", "--unorderedsetstring", 
                                              "get unordered_set string", 
                                              {}, {"aaa", "vvv", "bbb", 
                                              "rrr", "ttt"});
    cmd->add<std::unordered_set<int>>("-unint", "--unorderdsetint", 
                                      "get unordered_set int", {}, {44, 99});
    cmd->add<std::unordered_multiset<std::string>>("-unmstr", 
                                                   "--unorderedmsetstring", 
                                                   "get unordered_multiset string", 
                                                   {}, 
                                                   {"aaa", "vvv", "bbb", "rrr"});
    // add specified type of variable.
    // 1st argument is short name
    // 2nd argument is long name
    // 3rd argument is description
    // 4th argument is depends option
    // 5th argument is paramter ranage
    cmd->add<std::unordered_multiset<int>>("-unmint", "--unorderdmsetint", 
                                           "get unordered_multiset int", 
                                           {"-dix"}, {44, 99});
#endif
    cmd->add<std::multiset<std::string>>("-mmstr", "--multisetstring", 
                                         "get multiset string", 
                                         {}, {"aaa", "vvv", "bbb", 
                                         "rrr", "ttt"});
    cmd->add<std::multiset<int>>("-mmint", "--multisetint", "get multiset int", 
                                 {}, {44, 99});
    // add specified type of variable.
    // 1st argument is long name
    // 2nd argument is short name (no short name if '\0' specified)
    // 3rd argument is description
    // 4th argument is depends option
    cmd->add("-n", "--none", "get none", {"-s", "-us", "-i"});
    // add specified type of variable.
    // 1st argument is long name
    // 2nd argument is short name (no short name if '\0' specified)
    // 3rd argument is description
    // 4th argument is depends option
    // 5th argument is paramter ranage
    cmd->add<std::string>("-sns", "--singlestring", "get single string", 
                         {"-s", "-us", "-i"}, 
                         {"aaa", "vvv", "bbb", "rrr", "ttt"});
    cmd->add<int>("-sni1", "--singleint1", "get single int1", 
                 {"-s", "-us", "-i"}, {12, 33});
    cmd->add<int>("-sni2", "--singleint2", "get single int2", 
                 {"-s", "-us", "-i"}, {12, 33});
    cmd->add("-v", "--version", "get version");
 
    cmd->parse(false, argc, argv);

    Logger::setLevel(LogLevel::all);

    bool ret = cmd->get("--none");
    if (ret)
    {
        LOGGER << "--none enable";
    }

    ret = cmd->get("--version");
    if (ret)
    {
        LOGGER << "version: " << PROJECT_VERSION;
    }

    std::string singleStr;
    ret = cmd->get("--singlestring", singleStr);
    if (ret)
    {
        LOGGER << singleStr;
    }

    int singleInt1;
    ret = cmd->get("--singleint1", singleInt1);
    if (ret)
    {
        LOGGER << singleInt1;
    }

    int singleInt2;
    ret = cmd->get("--singleint2", singleInt2);
    if (ret)
    {
        LOGGER << singleInt2;
    }

    std::vector<short> sVector;
    ret = cmd->get("--short", sVector);
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

    std::multiset<std::string> strMultiset;
    ret = cmd->get("--multisetstring", strMultiset);
    if (ret)
    {
        for (auto &v : strMultiset)
        {
            LOGGER << v;
        }
    }

    std::multiset<int> intMultiset;
    ret = cmd->get("--multisetint", intMultiset);
    if (ret)
    {
        for (auto &v : intMultiset)
        {
            LOGGER << v;
        }
    }

    std::unordered_set<std::string> strUnorderedset;
    ret = cmd->get("--unorderedsetstring", strUnorderedset);
    if (ret)
    {
        for (auto &v : strUnorderedset)
        {
            LOGGER << v;
        }
    }

    std::unordered_set<int> intUnorderedset;
    ret = cmd->get("--unorderdsetint", intUnorderedset);
    if (ret)
    {
        for (auto &v : intUnorderedset)
        {
            LOGGER << v;
        }
    }
#if __GNUC__ > 6
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

    std::forward_list<std::string> strForwardList;
    ret = cmd->get("--forwardliststring", strForwardList);
    if (ret)
    {
        for (auto &v : strForwardList)
        {
            LOGGER << v;
        }
    }

    std::forward_list<int> intForwardList;
    ret = cmd->get("--forwardlistint", intForwardList);
    if (ret)
    {
        for (auto &v : intForwardList)
        {
            LOGGER << v;
        }
    }
    
    std::unordered_multiset<std::string> strUnorderedmultiset;
    ret = cmd->get("--unorderedmsetstring", strUnorderedmultiset);
    if (ret)
    {
        for (auto &v : strUnorderedmultiset)
        {
            LOGGER << v;
        }
    }

    std::unordered_multiset<int> intUnorderedmultiset;
    ret = cmd->get("--unorderdmsetint", intUnorderedmultiset);
    if (ret)
    {
        for (auto &v : intUnorderedmultiset)
        {
            LOGGER << LogFormat::addr << v;
        }
    }
#endif
}
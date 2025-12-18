#include "version.h"

#include "cmdline/cmdline.h"
#include "util/single.hpp"

#include <vector>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <forward_list>
#include <unordered_set>

using namespace lmc;

void getshort(std::vector<short> &v) {
    std::cout << "get short: ";
    for (auto &i : v) {
        std::cout << i << " ";  
    }
    std::cout << std::endl;
}

void getus(unsigned short &v) {
    std::cout << "get unsigned short: ";
    std::cout << v << std::endl;
}

void noparameter() {
    std::cout << "no parameter" << std::endl;
}

int parsecmdline(CmdLine *cmd, int argc, char *argv[]) {
    cmd->add("-s", "--short", "get short", getshort);
    cmd->add("-us", "--unsigedshorts", "get unsigned short", getus);
    cmd->add("-i", "--int", "get int", [] (int &v) {
        std::cout << "get int: ";
        std::cout << v << " ";  
        std::cout << std::endl;
    });
    cmd->add("-vi", "--vectorunsignedint", "get unsigned int", [] (std::vector<unsigned int> &v) {
        std::cout << "get unsigned int: ";
        for (auto &i : v) {
            std::cout << i << " ";  
        }
        std::cout << std::endl;
    }, {"-s", "-us", "-i"});
    cmd->add("-d", "--default", "no parameter", noparameter, {"-s", "-i"});
    cmd->add("-do", "--double", "get double", [] (std::vector<double> &data) {
        std::cout << "get vector double: ";
        for (auto &i : data) {
            std::cout << i << " ";  
        }
        std::cout << std::endl;
    });
    cmd->add("-n", "--none", "get none", [] {
        std::cout << "get none";
    });
    cmd->add("-l", "--long", "get long", [] (std::vector<long> &data) {
        std::cout << "get vector long: ";
        for (auto &i : data) {
            std::cout << i << " ";  
        }
        std::cout << std::endl;
    });
    cmd->add("-string", "--string", "get string", [] (std::vector<std::string> &data) {
        std::cout << "get vector string ";
        for (auto &i : data) {
            std::cout << i << " ";  
        }
        std::cout << std::endl;
    }, {}, {"aaa", "vvv", "bbb", "rrr", "ttt"});
    cmd->add("-ls", "--lstring", "get list string", [] (std::list<std::string> &data) {
        std::cout << "get list string ";
        for (auto &i : data) {
            std::cout << i << " ";  
        }
        std::cout << std::endl;
    }, {}, {"aaa", "vvv", "bbb", "rrr", "ttt"});
    cmd->add("-ss", "--sstring", "get set string", [] (std::set<std::string> &data) {
        std::cout << "get set string ";
        for (auto &i : data) {
            std::cout << i << " ";  
        }
        std::cout << std::endl;
    }, {}, {"aaa", "vvv", "bbb", "rrr", "ttt"});
    cmd->add("-si", "--sint", "get set int", [] (std::set<int> &data) {
        std::cout << "get set int ";
        for (auto &i : data) {
            std::cout << i << " ";  
        }
        std::cout << std::endl;
    }, {"-ss", "-us"}, {44, 99});
    cmd->add("-ds", "--dstring", "get deque string", [] (std::deque<std::string> &data) {
        std::cout << "get deque string ";
        for (auto &i : data) {
            std::cout << i << " ";  
        }
        std::cout << std::endl;
    }, {}, {"aaa", "vvv", "bbb", "rrr", "ttt"});
    cmd->add("-di", "--dint", "get deque int", [] (std::deque<int> &data) {
        std::cout << "get deque int ";
        for (auto &i : data) {
            std::cout << i << " ";  
        }
        std::cout << std::endl;
    }, {}, {44, 99});
#if __GNUC__ > 6
    cmd->add("-dsss", "--queuestring",  "get queue string", [] (std::queue<std::string> &data) {
        std::cout << "get queue string ";
        while (!data.empty()) {
            std::cout << data.front() << " ";
            data.pop();
        }
        std::cout << std::endl;
    }, {}, std::queue<std::string>({"aaa", "vvv", "bbb", "rrr", "ttt"}));
    cmd->add("-diss", "--queueint", "get queue int", [] (std::queue<int> &data) {
        std::cout << "get queue int ";
        while (!data.empty()) { 
            std::cout << data.front() << " ";
            data.pop();
        }
    }, {}, std::queue<int>({44, 99}));
    cmd->add("-dsa", "--stackstring", "get stack string", [] (std::stack<std::string> &data) {
        std::cout << "get stack string ";
        while (!data.empty()) {
            std::cout << data.top() << " ";
            data.pop();
        }
        std::cout << std::endl;
    }, {}, std::stack<std::string>({"aaa", "vvv", "bbb", "rrr", "ttt"}));
    cmd->add("-dix", "--stackint", "get stack int", [] (std::stack<int> &data) {
        std::cout << "get stack int ";
        while (!data.empty()) {
            std::cout << data.top() << " ";
            data.pop();
        }
        std::cout << std::endl;
    }, {}, std::stack<int>({44, 99}));
    cmd->add("-flstring",  "--forwardliststring", "get forward_list string", [](std::forward_list<std::string> &data) {
        std::cout << "get forward_list string ";
        for (const auto &i : data) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }, {}, {"aaa", "vvv", "bbb",  "rrr", "ttt"});
    cmd->add("-flint", "--forwardlistint",  "get forward_list int", [] (std::forward_list<int> &data) {
        std::cout << "get forward_list int ";
        for (const auto &i : data) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }, {}, {44, 99});
    cmd->add("-unstr", "--unorderedsetstring", "get unordered_set string", [](std::unordered_set<std::string> &data) {
        std::cout << "get unordered_set string ";
        for (const auto &i : data) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }, {}, {"aaa", "vvv", "bbb",  "rrr", "ttt"});
    cmd->add("-unint", "--unorderdsetint",  "get unordered_set int", [] (std::unordered_set<int> &data) {
        std::cout << "get unordered_set int ";
        for (const auto &i : data) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }, {}, {44, 99});
    cmd->add("-unmstr",  "--unorderedmsetstring",  "get unordered_multiset string", [](std::unordered_multiset<std::string> &data) {
        std::cout << "get unordered_multiset string ";
        for (const auto &i : data) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }, {},  {"aaa", "vvv", "bbb", "rrr"});
    // add specified type of variable.
    // 1st argument is short name
    // 2nd argument is long name
    // 3rd argument is description
    // 4th argument is depends option
    // 5th argument is paramter ranage
    cmd->add("-unmint", "--unorderdmsetint", "get unordered_multiset int", [](std::unordered_multiset<int> &data) {
        std::cout << "get unordered_multiset int ";
        for (const auto &i : data) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }, {"-dix"}, {44, 99});
#endif
    cmd->add("-mmstr", "--multisetstring", "get multiset string", [](std::multiset<std::string> &data) {
        std::cout << "get multiset string ";
        for (const auto &i : data) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }, {}, {"aaa", "vvv", "bbb",  "rrr", "ttt"});
    cmd->add("-mmint", "--multisetint", "get multiset int", [](std::multiset<int> &data) {
        std::cout << "get multiset int ";
        for (const auto &i : data) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }, {}, {44, 99});

    // add specified type of variable.
    // 1st argument is long name
    // 2nd argument is short name (no short name if '\0' specified)
    // 3rd argument is description
    // 4th argument is depends option
    // 5th argument is paramter ranage
    cmd->add("-sns", "--singlestring", "get single string", [](std::string &data) {
        std::cout << "get single string " << data << std::endl;
    },  {"-s", "-us", "-i"},  {"aaa", "vvv", "bbb", "rrr", "ttt"});
 
    cmd->parse(false, argc, argv);

    return 0;
}

int main(int argc, char *argv[]) {

    CmdLine *cmd = TypeSingle<CmdLine>::getInstance();
    try {
        parsecmdline(cmd, argc, argv);
    } catch(CmdLineError& e) {
        std::cerr << e.what() << '\n';
    }

    TypeSingle<CmdLine>::destory();
    return 0;
}
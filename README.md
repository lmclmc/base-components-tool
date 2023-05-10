# base-components-tool
目前支持线程池 定时器（支持多任务） log日志 cmdline 命令行解析 等功能 
## build
    ./build.sh
## cmdline
### normal usage
    #include "cmdline/cmdline.h"
    #include "util/single.hpp"
    #include "log/log.h"

    int main(int argv, char *argv)
    {
        CmdLine *cmd = TypeSingle<CmdLine>::getInstance();
        // add specified type of variable.
        // 1st argument is short name
        // 2nd argument is long name
        // 3rd argument is description
        // 4th argument is depends option
        // 5th argument is paramter ranage
        cmd->add<std::unordered_multiset<int>>("-unmint", "--unorderdmsetint", 
                                            "get unordered_multiset int", 
                                            {"-dix"}, {44, 99});
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
        //parse paramter argv
        cmd->parse(false, argc, argv);
        //get value
        std::string singleStr;
        bool ret = cmd->get("--singlestring", singleStr);
        ...
        ...
        ...
    }

### cmdline execution results
    ./build/install/demo/cmdline/cmdline 
    usage: ./build/install/demo/cmdline/cmdline [options] ...
    options:
    -s        , --short                  get short
                                            parameter range is [33 , 55]
    -us       , --us                     get unsigned short
    -i        , --int                    get int
    -ui       , --ui                     get unsigned int
                                            option depends [ -s -us -i ]
    -f        , --float                  get float
                                            parameter range is [23 , 100]
    -d        , --double                 get double
    -l        , --long                   get long
    -ul       , --ul                     get unsigned long
    -string   , --string                 get string
                                            parameter range is [ aaa vvv bbb rrr ttt ]
    -ls       , --lstring                get list string
                                            parameter range is [ aaa vvv bbb rrr ttt ]
    -ss       , --sstring                get set string
                                            parameter range is [ aaa bbb rrr ttt vvv ]
    -si       , --sint                   get set int
                                            option depends [ -ss -us ]
                                            parameter range is [44 , 99]
    -ds       , --dstring                get deque string
                                            parameter range is [ aaa vvv bbb rrr ttt ]
    -di       , --dint                   get deque int
                                            parameter range is [44 , 99]
    -dsss     , --queuestring            get queue string
                                            parameter range is [ aaa vvv bbb rrr ttt ]
    -diss     , --queueint               get queue int
                                            parameter range is [44 , 99]
    -dsa      , --stackstring            get stack string
                                            parameter range is [ ttt rrr bbb vvv aaa ]
    -dix      , --stackint               get stack int
                                            parameter range is [44 , 99]
    -flstring , --forwardliststring      get forward_list string
                                            parameter range is [ aaa vvv bbb rrr ttt ]
    -flint    , --forwardlistint         get forward_list int
                                            parameter range is [44 , 99]
    -mmstr    , --multisetstring         get multiset string
                                            parameter range is [ aaa bbb rrr ttt vvv ]
    -mmint    , --multisetint            get multiset int
                                            parameter range is [44 , 99]
    -unstr    , --unorderedsetstring     get unordered_set string
                                            parameter range is [ bbb rrr vvv ttt aaa ]
    -unint    , --unorderdsetint         get unordered_set int
                                            parameter range is [44 , 99]
    -unmstr   , --unorderedmsetstring    get unordered_multiset string
                                            parameter range is [ bbb rrr vvv aaa ]
    -unmint   , --unorderdmsetint        get unordered_multiset int
                                            parameter range is [44 , 99]
    -n        , --none                   get none
                                            option depends [ -s -us -i ]
    -sns      , --singlestring           get single string
                                            option depends [ -s -us -i ]
                                            parameter range is [ aaa vvv bbb rrr ttt ]
    -sni1     , --singleint1             get single int1
                                            option depends [ -s -us -i ]
                                            parameter range is [12 , 33]
    -sni2     , --singleint2             get single int2
                                            option depends [ -s -us -i ]
                                            parameter range is [12 , 33]
    -v        , --version                get version
    -h        , --help                   print help message

## timer
	./build/install/demo/timer/timer 
	usage: ./build/install/demo/timer/timer [options] ...
	options:
		-a        , --add                    add timer
		-c        , --clear                  delay sometime clear all timer
		-d        , --default                default mode
		-h        , --help                   print help message
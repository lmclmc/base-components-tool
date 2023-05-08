#include <iostream>
#include "threadpool/workqueue.h"
#include "timer/ltimer.h"
#include "util/single.hpp"
#include "cmdline/cmdline.h"
#include <unistd.h>

using namespace std;
using namespace lmc;

int main(int argc, char *argv[])
{
    CmdLine *cmdline = TypeSingle<CmdLine>::getInstance();

    cmdline->add<std::list<int>>("-a", "--add", "add timer");
    cmdline->add<int>("-c", "--clear", "delay sometime clear all timer");
    cmdline->add("-d", "--default", "default mode");

    cmdline->parse(false, argc, argv);

    LTimer *t = TypeSingle<LTimer>::getInstance();
    t->startTimer();

    bool ret = cmdline->get("--default");
    if (ret)
    {
         uint64_t timer100 = t->setTimer(100, [] {
            static uint64_t tvS = 0;
            static uint64_t tvE = 0;
            tvS = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
            usleep(70000);
            cout << "100ms delay test " << (tvS - tvE)/1000 << endl;
            tvE = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
        });

        t->setTimer(1000, [] {
            static uint64_t tvS = 0;
            static uint64_t tvE = 0;
            tvS = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
            cout << "1000ms delay test " << (tvS - tvE)/1000 << endl;
            tvE = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
        });

        uint64_t timer300 = t->setTimer(300, [] {
                static uint64_t tvS = 0;
                static uint64_t tvE = 0;
                tvS = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
                cout << "300ms delay test " << (tvS - tvE)/1000 << endl;
                tvE = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
            });

        t->setTimer(700, [] {
            static uint64_t tvS = 0;
            static uint64_t tvE = 0;
            tvS = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
            cout << "700ms delay test " << (tvS - tvE)/1000 << endl;
            tvE = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
        });

        t->setTimer(1500, [] {
            static uint64_t tvS = 0;
            static uint64_t tvE = 0;
            tvS = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
            cout << "1500ms delay test " << (tvS - tvE)/1000 << endl;
            tvE = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
        });

        t->setTimer(30000, [] {
        static uint64_t tvS = 0;
            static uint64_t tvE = 0;
            tvS = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
            cout << "3000ms delay test " << (tvS - tvE)/1000 << endl;
            tvE = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
        });

        sleep(10);
        std::cout << "remove 100ms and 300ms delay task" << std::endl;
        t->removeTimer(timer100);
        t->removeTimer(timer300);

        sleep(10);
        t->clearTimer();
    }
    else
    {
        std::list<int> sList;
        bool ret = cmdline->get("--add", sList);
        if (!ret)
            exit(0);

        for (auto &l : sList)
        {
            t->setTimer(l, [l] {
                static uint64_t tvS = 0;
                static uint64_t tvE = 0;
                tvS = std::chrono::system_clock::now().time_since_epoch().count() / 1000;

                cout << l <<"ms delay test " << (tvS - tvE)/1000 << endl;
                tvE = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
            });
        }
        int time;
        ret = cmdline->get("--clear", time);
        if (ret)
        {
            usleep(time * 1000);
            t->clearTimer();
        }
    }

    sleep(1000000000);
    TypeSingle<LTimer>::destory();
    pause();

    return 0;
}

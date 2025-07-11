#include <iostream>
#include "version.h"
#include "threadpool/workqueue.h"
#include "timer/ltimer.h"
#include "util/single.hpp"
#include "cmdline/cmdline.h"
#include <unistd.h>

using namespace std;
using namespace lmc;


int main(int argc, char *argv[]) {
    CmdLine *cmdline = TypeSingle<CmdLine>::getInstance();

    cmdline->add<std::list<int>>("-a", "--add", "add timer");
    cmdline->add<int>("-c", "--clear", "delay sometime clear all timer");
    cmdline->add("-d", "--default", "default mode");
    cmdline->add("-v", "--version", "get version");
    cmdline->parse(false, argc, argv);

    bool ret = cmdline->get("--version");
    if (ret) {
        cout << "version: " << PROJECT_VERSION << endl;
        return 0;
    }

    LTimer *t = TypeSingle<LTimer>::getInstance();
    t->startTimer();

    ret = cmdline->get("--default");
    if (ret) {
         uint64_t timer100 = t->setTimer(100, [] {
            static uint64_t tvS = 0;
            static uint64_t tvE = 0;
            tvS =static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
            usleep(70000);
            cout << "100ms delay test " << (tvS - tvE)/1000 << endl;
            tvE =static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
        });

        t->setTimer(1000, [] {
            static uint64_t tvS = 0;
            static uint64_t tvE = 0;
            tvS =static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
            cout << "1000ms delay test " << (tvS - tvE)/1000 << endl;
            tvE =static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
        });

        uint64_t timer300 = t->setTimer(300, [] {
                static uint64_t tvS = 0;
                static uint64_t tvE = 0;
                tvS = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
                cout << "300ms delay test " << (tvS - tvE)/1000 << endl;
                tvE = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
            });

        t->setTimer(700, [] {
            static uint64_t tvS = 0;
            static uint64_t tvE = 0;
            tvS = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
            cout << "700ms delay test " << (tvS - tvE)/1000 << endl;
            tvE = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
        });

        t->setTimer(1500, [] {
            static uint64_t tvS = 0;
            static uint64_t tvE = 0;
            tvS = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
            cout << "1500ms delay test " << (tvS - tvE)/1000 << endl;
            tvE = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
        });

        t->setTimer(30000, [] {
        static uint64_t tvS = 0;
            static uint64_t tvE = 0;
            tvS = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
            cout << "3000ms delay test " << (tvS - tvE)/1000 << endl;
            tvE = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
        });

        sleep(10);
        std::cout << "remove 100ms and 300ms delay task" << std::endl;
        t->removeTimer(timer100);
        t->removeTimer(timer300);

        sleep(10);
        t->clearTimer();
    } else {
        std::list<int> sList;
        ret = cmdline->get("--add", sList);
        if (!ret)
            exit(0);

        typedef struct ele_ {
            int count = 0;
            std::string name = "";
            double tvS;
        } ele;
        std::list<ele> eleList;
        for (auto &l : sList) {
            eleList.emplace_back(ele());
            auto last = &eleList.back();
            (*last).name = std::to_string(l);
            (*last).count = 10000000;
            cout << last << endl;
            t->setTimer(l, [last] {
                if ((*last).count++ > 10000000) {
                    (*last).tvS = static_cast<double>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
                    (*last).count = 0;
                }

                double timestamp = static_cast<double>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000 - (*last).tvS;
                cout << "timer name: " << (*last).name << " duration per count: " << timestamp/1000/(*last).count << " ms"<< endl;
            });
        }

        int time;
        ret = cmdline->get("--clear", time);
        if (ret) {
            usleep(static_cast<__useconds_t>(time) * 1000);
            t->clearTimer();
        }
    }

    sleep(10);
    TypeSingle<LTimer>::destory();
    TypeSingle<CmdLine>::destory();
    cout << "timer demo end" << endl;
    // Pause to see the output
    pause();

    return 0;
}

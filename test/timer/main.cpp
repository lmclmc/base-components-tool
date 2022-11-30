#include <iostream>
#include "threadpool/workqueue.h"
#include "timer/ltimer.h"
#include "single/single.hpp"
#include <unistd.h>
using namespace std;
using namespace lmc;

int main()
{
    LTimer *t = TypeSingle<LTimer>::getInstance();
    t->startTimer();
    
    uint64_t timer100 = t->setTimer(100, [] {
            static uint64_t tvS = 0;
            static uint64_t tvE = 0;
            tvS = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
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
     std::cout << "yyyyyyyyy" << std::endl;


    sleep(30);
    std::cout << "remove 100ms and 300ms delay task" << std::endl;
    t->removeTimer(timer100);
    t->removeTimer(timer300);

    pause();

    return 0;
}

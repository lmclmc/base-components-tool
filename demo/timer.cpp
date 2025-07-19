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
    TypeSingle<LTimer>::getInstance()->startTimer();

    cmdline->add("-a", "--add", "add timer", [] (std::list<int> &data) {
        typedef struct ele_ {
            int count = 0;
            std::string name = "";
            double tvS;
        } ele;
        std::list<ele> eleList;
        for (auto &l : data) {
            eleList.emplace_back(ele());
            auto last = &eleList.back();
            (*last).name = std::to_string(l);
            (*last).count = 10000000;
            cout << last << endl;
            TypeSingle<LTimer>::getInstance()->setTimer(l, [last] {
                if ((*last).count++ > 10000000) {
                    (*last).tvS = static_cast<double>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000;
                    (*last).count = 0;
                }

                double timestamp = static_cast<double>(std::chrono::system_clock::now().time_since_epoch().count()) / 1000 - (*last).tvS;
                cout << "timer name: " << (*last).name << " duration per count: " << timestamp/1000/(*last).count << " ms"<< endl;
            });
        }
    });

    cmdline->add("-c", "--creazy", "creazy mode", [] {
        cout << "creazy mode" << endl;
        for (int i = 0; i < 100000; i++) {
            TypeSingle<LTimer>::getInstance()->setTimer(10, [] {
                TypeSingle<LTimer>::getInstance()->setTimer(10, [] {
                }, 10);
            }, 10);
        }
    });
    cmdline->add("-v", "--version", "get version", [] {
        cout << "version: " << PROJECT_VERSION << endl;
        exit(0);
    });

    cmdline->parse(false, argc, argv);

    sleep(10);
    TypeSingle<LTimer>::destory();
    TypeSingle<CmdLine>::destory();
    cout << "timer demo end" << endl;
    // Pause to see the output
    //pause();

    return 0;
}

#include <iostream>
#include "version.h"
#include "threadpool/workqueue.h"
#include "timer/ltimer.h"
#include "util/single.hpp"
#include "cmdline/cmdline.h"
#include <unistd.h>

#include <iostream>
#include <future>
#include <thread>

using namespace std;
using namespace lmc;

template <typename F>
struct FunctionTraits;

template <typename ...Args>
struct FunctionTraits<std::function<void(Args...)>>  {
public:
    FunctionTraits(const std::function<void(Args...)> &func,
                    std::promise<int> &&promise_) : cb(func), promise1(std::move(promise_)) {
        std::cout << "FunctionTraits constructor called" << std::endl;
    }
    int asdd = 2333;
    std::promise<int> promise1;

    std::function<void(Args...)> cb;
    static void ccb(Args... args) {
        std::cout << "FunctionTraits::ccb called with args: " << std::endl;
     
    }
    std::function<void(Args...)> asd = [this] (Args... args) {
        std::cout << "FunctionTraits::asd called with args: " << std::endl;
      this->cb(args...);
      this->promise1.set_value(0); // 设置promise的值，表示任务完成
    };
};
 
template <typename AsyncFunc, typename AsyncCallback>
void runAsyncAsSyncTask(const AsyncFunc &asyncFunc, const AsyncCallback &callback) {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    using ArgType = typename function_traits<AsyncFunc>::ArgType;
    FunctionTraits<ArgType> asd(callback, std::move(promise));
  //  asd.cb = callback;
   // asd.asd = callback;
  //  asd.promise = std::move(promise);
    asyncFunc(asd.asd);
    future.get(); // 等待异步任务完成
 //  asyncFunc(callback);
  //  function_traits<AsyncCallback>
    
}

void asyncTask(std::function<void(std::string, int, std::string, double)> func) {
    TypeSingle<LTimer>::getInstance()->setTimer(1000, [func] {
        printf("vvvvvvvvvvvvv\n");
        func("how are you?", 11111, "hello", 3.14);
      //  printf("wwwwwwwwwwwwwwwwwwwwwwwwwww\n");
    }, 1);
}
int main(int argc, char *argv[]) {
    CmdLine *cmdline = TypeSingle<CmdLine>::getInstance();
    TypeSingle<LTimer>::getInstance()->startTimer();
    asyncTask([] (std::string str, int num, std::string str2, double d) {
        cout << "This is a synchronous task, received: " << str << ", " << num << ", " << str2 << ", " << d << endl;
    });

 //   sleep(2);
    cout << "timer demo start" << endl;

    runAsyncAsSyncTask(asyncTask, [] (std::string str, int num, std::string str2, double d) {
        cout << "This is a synchronous task, received: " << str << ", " << num << ", " << str2 << ", " << d << endl;
    });
        
    std::cout << "This is a synchronous task." << std::endl;
 //   pause();
    exit(0);

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

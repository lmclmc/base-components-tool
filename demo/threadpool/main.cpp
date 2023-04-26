#include <iostream>
#include <unistd.h>

#include "threadpool/workqueue.h"
#include "timer/ltimer.h"
#include "util/single.hpp"

using namespace std;
using namespace lmc;

void asd(){
    static int num = 0;
    if (num % 100000 == 0) cout << num << endl;
         num++;

}

int test(int num)
{
    return num;
}

int main()
{
    WorkQueue *a = TypeSingle<WorkQueue>::getInstance(MutexType::Mutex);
    for (int i = 0; i < 1000000; i++)
    {
        a->addTask(asd);
    }
    std::this_thread::sleep_for(std::chrono::microseconds(2000000));

    auto ret1 = a->addTask(test, 1111111);
    auto ret2 = a->addTask(test, 2222222);
    auto ret3 = a->addTask(test, 333333);

    auto ret4 = a->addTask([](int num){
        std::this_thread::sleep_for(std::chrono::microseconds(5000000));
        return num;
    }, 444444);
    auto ret5 = a->addTask([](int num){
        std::this_thread::sleep_for(std::chrono::microseconds(5000000));
        return num;
    }, 5555555);
    auto ret6 = a->addTask([](int num){
        std::this_thread::sleep_for(std::chrono::microseconds(5000000));
        return num;
    }, 66666666);
    
    std::cout << "ret = " <<  ret1.get() << std::endl;
    std::cout << "ret = " <<  ret2.get() << std::endl;
    std::cout << "ret = " <<  ret3.get() << std::endl;
    std::cout << "ret = " <<  ret4.get() << std::endl;
    std::cout << "ret = " <<  ret5.get() << std::endl;
    std::cout << "ret = " <<  ret6.get() << std::endl;
    pause();
    return 0;
}

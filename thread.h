#ifndef THREAD_H
#define THREAD_H

#include <iostream>
#include <condition_variable>
#include <thread>
#include <atomic>

using namespace std;

namespace lmc{
    class Thread
    {
    public:
        Thread();
        ~Thread();

        void start();

    protected:
        virtual void run();

    private:
        void destory();

    private:
        condition_variable condition;
        atomic<long> conditionStatus;
        atomic<bool> bStop;
        thread t;
    };

    class testThread : public Thread{
    public:
        testThread();

        void test();

    protected:
        void run();
    };
}

#endif // THREAD_H

#ifndef THREAD_H
#define THREAD_H

#include <iostream>
#include <condition_variable>
#include <thread>

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
        condition_variable condition;
        bool conditionStatus;
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

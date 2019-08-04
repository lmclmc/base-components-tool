#include "thread.h"

#include <mutex>

using namespace lmc;

Thread::Thread()
{
    t = thread([this]{
        mutex localMutex;
        unique_lock<mutex> localLock(localMutex);
        while (true){
            condition.wait(localLock, [this]{return conditionStatus;});
            conditionStatus = false;
            run();
        }
    });
}

Thread::~Thread()
{

}

void Thread::start()
{
    conditionStatus = true;
    condition.notify_one();
}

void Thread::run()
{

}

testThread::testThread()
{
    start();
}

void testThread::test()
{
    start();
}

void testThread::run()
{
    cout << "gergaaa" << endl;
}

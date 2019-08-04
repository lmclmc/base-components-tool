#include "thread.h"

#include <mutex>

using namespace lmc;

Thread::Thread()
{
    bStop = true;
    conditionStatus = 0;

    t = thread([this]{
        mutex localMutex;
        unique_lock<mutex> localLock(localMutex);
        while (bStop){
            condition.wait(localLock, [this]{return conditionStatus?conditionStatus--:false;});
            run();
        }
    });
}

Thread::~Thread()
{
    destory();
}

void Thread::start()
{
    conditionStatus++;
    condition.notify_one();
}

void Thread::run()
{

}

void Thread::destory()
{
    bStop = false;
    start();
    t.join();
}

testThread::testThread()
{
}

void testThread::test()
{
    start();
}
#include <unistd.h>
void testThread::run()
{
    usleep(100000);
    static int num = 0;
    cout << "gergaaa   " << num++ << endl;
}

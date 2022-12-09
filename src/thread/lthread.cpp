#include "lthread.h"

#include <mutex>

using namespace lmc;
using namespace std;

Thread::Thread()
{
    bStop = true;
    cStatus = 0;

    t = thread([this]{
        mutex localMutex;
        unique_lock<mutex> localLock(localMutex);
        while (bStop){
            c.wait(localLock, [this]{return cStatus?cStatus--:false;});
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
    cStatus++;
    c.notify_one();
}

void Thread::destory()
{
    bStop = false;
    start();
    t.join();
}

#include "thread.h"

#include <mutex>

using namespace lmc;

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

WorkQueue::WorkQueue() :
    queue(make_shared<workqueue>())
{
    start();
}

WorkQueue::~WorkQueue()
{

}

void WorkQueue::run()
{
//    mutex localMutex;
//    unique_lock<mutex> localLock(localMutex);
    shared_ptr<function<void()>> f;
//    shared_ptr<workqueue> q = queue;
//    while (true){
//        condition.wait(localLock, [&q, &f]{
//            return q->read(&f);
//        });
//        (*f)();
//    }
    if (queue->read(&f))(*f)();
    else cout << "err" << endl;

}

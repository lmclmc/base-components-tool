#include "workqueue.h"

using namespace lmc;

void SMutex::setMutexType(MutexType m)
{
    mMutexType = m;
}

void SMutex::lock()
{
    switch (mMutexType)
    {
        case MutexType::Mutex:
        mMutex.lock();
        break;
        case MutexType::Spin:
        mSpinMutex.lock();
        break;
        case MutexType::None:
        return;
        break;
    }
}

void SMutex::unlock()
{
    switch (mMutexType)
    {
        case MutexType::Mutex:
        mMutex.unlock();
        break;
        case MutexType::Spin:
        mSpinMutex.unlock();
        break;
        case MutexType::None:
        return;
        break;
    }
}

WorkQueue::WorkQueue(MutexType m)
{
    mutex.setMutexType(m);
    start();
}

WorkQueue::~WorkQueue()
{
    while (!workqueue.empty()) 
        workqueue.pop();
}

void WorkQueue::run()
{
    if (workqueue.empty()) return;

    mutex.lock();
    function<void()> f = move(workqueue.front());
    workqueue.pop();
    mutex.unlock();

    f();
}

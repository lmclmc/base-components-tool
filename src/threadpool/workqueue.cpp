#include "workqueue.h"

using namespace lmc;

void SMutex::setMutexType(MutexType m) {
    mMutexType = m;
}

void SMutex::lock() {
    switch (mMutexType) {
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

void SMutex::unlock() {
    switch (mMutexType){
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

WorkQueue::WorkQueue(MutexType m) {
    mutex.setMutexType(m);
}

WorkQueue::~WorkQueue() {
    mutex.lock();
    while (!workqueue.empty()) 
        workqueue.pop();

    //将基类的线程销毁操作转交给派生类执行，主要原因是派生类在基类销毁前销毁,
    //导致基类的纯虚函数run无定义，然而这个时候基类的线程有可能会运行到run函数，
    //这就会导致程序崩溃。所以派生类提前销毁基类线程，避免纯虚函数被调用的问题.
    destory();
    mutex.unlock();
}

void WorkQueue::run() {
    mutex.lock();
    if (workqueue.empty()) {
        mutex.unlock();
        return;
    }

    auto f = move(workqueue.front());
    workqueue.pop();
    mutex.unlock();

    f();
}

void WorkQueue::stopWorkQueue() {
    stop();
}
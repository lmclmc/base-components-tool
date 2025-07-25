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
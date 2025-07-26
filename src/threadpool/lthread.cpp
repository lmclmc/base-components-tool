#include "lthread.h"

#include <condition_variable>
#include <atomic>
#include <thread>
#include <mutex>

#define SIZE (10000)

using namespace lmc;
using namespace std;

class Thread::Impl {
public:
    std::condition_variable c;
    std::atomic<bool> bNotify; //防止虚假唤醒
    std::atomic<bool> bStop;   //线程是否停止
    std::thread t;
};

Thread::Thread() : pImpl(std::make_unique<Impl>()) {
    pImpl->bStop = true;
    pImpl->bNotify = false;

    pImpl->t = thread([this] {
        mutex localMutex;
        unique_lock<mutex> localLock(localMutex);
        while (1){
            // 等待通知
            pImpl->c.wait(localLock, [this] {
                return pImpl->bNotify.load();
            });

            if (!pImpl->bStop)
                return;

            run();
        }
    });
}

Thread::~Thread() {}

void Thread::start() {
    pImpl->bNotify.store(true);
    pImpl->c.notify_one();
}

void Thread::destory() {
    pImpl->bStop = false;
    start();
    pImpl->t.join();
}
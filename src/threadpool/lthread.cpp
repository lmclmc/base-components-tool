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
    std::atomic<long> cStatus;
    std::atomic<bool> bStop;
    std::thread t;
};

Thread::Thread() : pImpl(std::make_unique<Impl>()) {
    pImpl->bStop = true;
    pImpl->cStatus = 0;

    pImpl->t = thread([this] {
        mutex localMutex;
        unique_lock<mutex> localLock(localMutex);
        while (1){
            pImpl->c.wait(localLock, [this] {
                return pImpl->cStatus ? pImpl->cStatus-- : false;
            });

            if (!pImpl->bStop)
                return;

            run();
        }
    });
}

Thread::~Thread() {
    pImpl->bStop = false;
    start();
    pImpl->t.join();
}

void Thread::start() {
    pImpl->cStatus++;
    pImpl->c.notify_one();
}
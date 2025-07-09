#include "ltimer.h"
#include "util/uuid.hpp"
#include "threadpool/workqueue.h"
#include "util/spinmutex.hpp"

#include <thread>
#include <list>

using namespace lmc;
using namespace std::chrono;

#define DELAY_TIME (100000)

class LTimer::Impl {
public:
    Impl() : bStatus(false),
             w(make_shared<WorkQueue>(MutexType::None)) {}

    void task() {
        w->addTask([this] {
        mutex.lock();
        int64_t now = steady_clock::now().time_since_epoch().count() / 1000;
        int64_t lastMinStamp = now + DELAY_TIME;
        //更新所有任务的时间戳
        for (auto it = this->taskList.begin();
                it != this->taskList.end();) {
            if (it->timeStamp <= now) {
                if (it->count > 0)
                    it->count--;
                //当准备时间小于0时，将任务加入到就绪队列里面
                this->taskQueue.emplace_back(it->task);

                while (it->timeStamp <= now)
                    it->timeStamp += it->timeout;
            }

            //取所有任务里面最小的准备时间。
            if (it->timeStamp < lastMinStamp)
                lastMinStamp = it->timeStamp;

            if (it->count == 0) {
                //当计数为0时，移除该任务。
                it = this->taskList.erase(it);
            }
            else {
                it++;
            }
        }

        mutex.unlock();

        std::this_thread::sleep_for(microseconds(lastMinStamp-now)/2);

        for (auto &t : this->taskQueue)
            t();

        this->taskQueue.clear();

        if (!this->bStatus)
            return;

        this->task();
    });
    }
    typedef struct TaskNode_ {
        TaskNode_(int64_t timeout_,
                  const function<void()> &task_,
                  int64_t count_,
                  uint64_t uuid_) : timeout(timeout_),
                                    timeStamp(steady_clock::now().time_since_epoch().count() / 1000),
                                    count(count_),
                                    uuid(uuid_),
                                    task(task_) {}

        int64_t timeout;  //任务超时时间
        int64_t timeStamp; //任务时间戳
        int64_t count;    //任务执行次数，-1代表无限执行
        uint64_t uuid;    //任务uuid
        function<void()> task;
    } TaskNode;

    list<TaskNode> taskList;
    list<function<void()>> taskQueue;

    atomic<bool> bStatus;  //是否开启定时器

    shared_ptr<WorkQueue> w;
    
    SpinMutex mutex;
};

LTimer::LTimer() : pImpl(std::make_unique<Impl>()) {}

LTimer::~LTimer() {
    clearTimer();
    stopTimer();
    pImpl->w = nullptr;
}

uint64_t LTimer::setTimer(int64_t time, 
                          const function<void()> &f, 
                          int64_t count) {
    if (time <= 0)
        return 0;
        
    uint64_t uuid = UUID::generateUuid();
    pImpl->mutex.lock();
    pImpl->taskList.emplace_back(time * 1000, f, count, uuid);
    pImpl->mutex.unlock();
    return uuid;
}

void LTimer::removeTimer(uint64_t uuid) {
    pImpl->mutex.lock();
    for (auto it = pImpl->taskList.begin(); it != pImpl->taskList.end();) {
        if (it->uuid == uuid) {
            it = pImpl->taskList.erase(it);
        }
        else {
            it++;
        }
    }

    pImpl->mutex.unlock();
}

void LTimer::clearTimer() {
    pImpl->mutex.lock();
    pImpl->taskList.clear();
    pImpl->mutex.unlock();
}

void LTimer::startTimer() {
    bool expect = false;
    if (!pImpl->bStatus.compare_exchange_strong(expect, true)) {
        return;
    }

    pImpl->task();
}

void LTimer::stopTimer() {
    pImpl->bStatus.store(false);
}
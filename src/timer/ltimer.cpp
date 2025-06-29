#include "ltimer.h"
#include "util/uuid.hpp"

using namespace std::chrono;

#define DELAY_TIME (100000)

LTimer::LTimer() : bStatus(false),
                   timeStamp(0),
                   tmpTimeStamp(DELAY_TIME),
                   w(make_shared<WorkQueue>(MutexType::None)) {}

LTimer::~LTimer() {
    mutex.lock();
    taskList.clear();
    taskQueue.clear();
    mutex.unlock();
    stopTimer();
    w = nullptr;
}

uint64_t LTimer::setTimer(int64_t time, 
                          const function<void()> &f, 
                          int64_t count) {
    if (time <= 0)
        return 0;
        
    uint64_t uuid = UUID::generateUuid();
    mutex.lock();
    taskList.emplace_back(time * 1000, time * 1000, f, count, uuid);
    tmpTimeStamp = tmpTimeStamp > time*1000 ? time*1000 : tmpTimeStamp;
    mutex.unlock();
    return uuid;
}

void LTimer::removeTimer(uint64_t uuid) {
    mutex.lock();
    for (auto it = taskList.begin(); it != taskList.end();) {
        if (it->uuid == uuid) {
            it = taskList.erase(it);
        }
        else {
            it++;
        }
    }

    mutex.unlock();
}

void LTimer::clearTimer() {
    mutex.lock();
    taskList.clear();
    tmpTimeStamp = DELAY_TIME;
    mutex.unlock();
}

void LTimer::startTimer() {
    bool expect = false;
    if (!bStatus.compare_exchange_strong(expect, true)) {
        return;
    }
    
    tvS = system_clock::now().time_since_epoch().count() / 1000;
    tvE = tvS;

    task();
}

void LTimer::stopTimer() {
    bStatus.store(false);
}

void LTimer::task() {
    w->addTask([this] {
        mutex.lock();
        static int64_t lastMinStamp = this->tmpTimeStamp;
        this->timeStamp = lastMinStamp;
        lastMinStamp = this->tmpTimeStamp;

        //更新所有任务的准备时间
        for (auto it = this->taskList.begin();
                it != this->taskList.end();) {
            it->time -= this->timeStamp;

            if (it->time <= 0) {
                if (it->count > 0)
                    it->count--;
                //当准备时间小于0时，将任务加入到就绪队列里面
                this->taskQueue.emplace_back(it->task);
                it->time = it->maxTime;
            }

            //取所有任务里面最小的准备时间。
            if (it->time < lastMinStamp)
                lastMinStamp = it->time;

            if (it->count == 0) {
                //当计数为0时，移除该任务。
                it = this->taskList.erase(it);
            }
            else {
                it++;
            }
        }
        //当没有定时器任务时，将临时延迟时间戳置为DELAY_TIME
        if (this->taskList.size() == 0) {
            tmpTimeStamp = DELAY_TIME;
        }

        mutex.unlock();

        //为了提高定时精确度，消除任务运行耗时造成的时间损耗，使用该变量。
        this->tvS = system_clock::now().time_since_epoch().count() / 1000;

        std::this_thread::sleep_for(microseconds(this->timeStamp + 
                                                 this->tvE - this->tvS));

        this->tvE = system_clock::now().time_since_epoch().count() / 1000;

        for (auto &t : this->taskQueue) {
            t();
        }

        this->taskQueue.clear();

        if (!this->bStatus)
            return;

        this->task();
    });
}

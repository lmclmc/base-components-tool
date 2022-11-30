#include "ltimer.h"
#include "single/uuid.hpp"

using namespace std;

LTimer::LTimer() : bStatus(false),
                   w(make_shared<WorkQueue>()),
                   timeStamp(0),
                   tmpTimeStamp(1000000)
{
}

uint64_t LTimer::setTimer(uint64_t time, const function<void()> &f, int64_t count)
{
    uint64_t uuid = UUID::generateUuid();
    mutex.lock();
    taskList.emplace_back(time * 1000, time * 1000, f, count, uuid);
    tmpTimeStamp = tmpTimeStamp > time*1000 ? time*1000 : tmpTimeStamp;
    mutex.unlock();
    return uuid;
}

void LTimer::removeTimer(uint64_t uuid)
{
    mutex.lock();
    for (auto it = taskList.begin(); it != taskList.end();)
    {
        if (it->uuid == uuid)
        {
            it = taskList.erase(it);
        }
        else
        {
            it++;
        }
    }
    mutex.unlock();
}

void LTimer::clearTimer()
{
    mutex.lock();
    taskList.clear();
    mutex.unlock();
}

void LTimer::startTimer()
{
    bStatus = true;

    tvS = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
    tvE = tvS;

    task();
}

void LTimer::stopTimer()
{
    bStatus = false;
}

void LTimer::task()
{
    w->addTask([this] {
        if (this->taskList.size() > 0)
        {
            mutex.lock();
            this->timeStamp = this->tmpTimeStamp;
            this->tmpTimeStamp = 1000000;

            //更新所有任务的准备时间
            for (auto it = this->taskList.begin();
                 it != this->taskList.end();)
            {
                it->time -= this->timeStamp;

                if (it->time <= 0)
                {
                    if (it->count > 0)
                        it->count--;
                    //当准备时间小于0时，将任务加入到就绪队列里面
                    this->taskQueue.emplace_back(it->task);
                    it->time = it->maxTime;

                }

                //取所有任务里面最小的准备时间。
                if (it->time < this->tmpTimeStamp)
                    this->tmpTimeStamp = it->time;

                if (it->count == 0)
                {
                    //当计数为0时，移除该任务。
                    it = this->taskList.erase(it);
                }
                else
                {
                    it++;
                }
            }

            mutex.unlock();

            //为了提高定时精确度，消除任务运行耗时造成的时间损耗，使用该变量。
            this->tvS = std::chrono::system_clock::now().time_since_epoch().count() / 1000;

            std::this_thread::sleep_for(std::chrono::microseconds(this->timeStamp + this->tvE - this->tvS));

            this->tvE = std::chrono::system_clock::now().time_since_epoch().count() / 1000;

            for (auto &t : this->taskQueue)
            {
                t();
            }

            this->taskQueue.clear();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
        }

        if (!this->bStatus)
            return;

        this->task();
    });
}

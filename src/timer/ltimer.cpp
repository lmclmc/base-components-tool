#include "ltimer.h"

using namespace std;

LTimer::LTimer() : bStatus(false),
                   w(make_shared<WorkQueue>()),
                   timeStamp(0)
{
}

void LTimer::setTimer(uint64_t time, const function<void()> &f)
{
    taskList.emplace_back(time * 1000, time * 1000, f);
}

void LTimer::clearTimer()
{
    taskList.clear();
}

void LTimer::startTimer()
{
    if (taskList.size() <= 0)
        return;
    bStatus = true;

    tvS = std::chrono::system_clock::now().time_since_epoch().count();

    task();
}

void LTimer::stopTimer()
{
    bStatus = false;
}

void LTimer::task()
{
    w->addTask([this] {
        this->timeStamp = this->taskList.front().maxTime;

        for (auto &l : this->taskList)
        {
            if (this->timeStamp > l.time)
            {
                this->timeStamp = l.time;
            }
        }

        for (auto &l : this->taskList)
        {
            l.time -= this->timeStamp;
            if (l.time <= 0)
            {
                this->taskQueue.emplace_back(l.task);
                l.time = l.maxTime;
            }
        }

        this->tvS = std::chrono::system_clock::now().time_since_epoch().count() / 1000;

        std::this_thread::sleep_for(std::chrono::microseconds(this->timeStamp + this->tvE - this->tvS));

        this->tvE = std::chrono::system_clock::now().time_since_epoch().count() / 1000;

        for (auto &t : this->taskQueue)
        {
            t();
        }

        this->taskQueue.clear();

        if (!this->bStatus)
            return;

        this->task();
    });
}
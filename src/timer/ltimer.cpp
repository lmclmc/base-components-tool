#include "ltimer.h"

using namespace std;

LTimer::LTimer() : bStatus(false),
                   w(make_shared<WorkQueue>()),
                   timeStamp(0)
{
}

void LTimer::setTimer(uint64_t time, const function<void()> &f, int64_t count)
{
    taskList.emplace_back(time * 1000, time * 1000, f, count);
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

        list<TaskNode>::iterator it = this->taskList.begin();
        auto &ll = this->taskList;
        for (auto it = this->taskList.begin();
             it != ll.end();)
        {
            it->time -= this->timeStamp;
            if (it->time <= 0)
            {
                it->count--;
                this->taskQueue.emplace_back(it->task);
                it->time = it->maxTime;
            }

            if (it->count == 0)
            {
                it = this->taskList.erase(it);
            }
            else
            {
                it++;
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
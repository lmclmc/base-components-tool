#include <sys/time.h>
#include <unistd.h>

#include "ltimer.h"

using namespace std;

LTimer::LTimer() :
    bStatus(false),
    w(make_shared<WorkQueue>()),
    timeStamp(0)
{}

void LTimer::setTimer(uint64_t time, const function<void()> &f)
{
    taskList.emplace_back(time*1000, time*1000, f);
}

void LTimer::startTimer()
{
    if (taskList.size() <= 0) return;
    bStatus = true;

    gettimeofday(&tvS, NULL);

    task();
}

void LTimer::stopTimer()
{
    bStatus = false;
}

void LTimer::task()
{
    w->addTask([this]{
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

        gettimeofday(&this->tvE, NULL);
        ::usleep(this->timeStamp -
                (this->tvE.tv_sec - this->tvS.tv_sec) * 1000000 -
                (this->tvE.tv_usec - this->tvS.tv_usec));

        gettimeofday(&this->tvS, NULL);

        for (auto &t : this->taskQueue)
        {
            t();
        }

        this->taskQueue.clear();

        if (!this->bStatus) return;

        this->task();
    });
}
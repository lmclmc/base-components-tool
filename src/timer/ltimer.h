#ifndef LTIMER_H_
#define LTIMER_H_

#include <memory>
#include <functional>
#include <list>

#include "threadpool/workqueue.h"

using namespace std;
using namespace lmc;

namespace lmc
{

class LTimer
{
public:
    LTimer();

    ~LTimer() = default;

    void setTimer(uint64_t, const function<void()> &);

    void startTimer();
    void stopTimer();
    void clearTimer();

private:
    void task();

private:
    typedef struct TaskNode_
    {
        TaskNode_(uint64_t time_, uint64_t maxTime_, function<void()> task_) : time(time_),
                                                                               maxTime(maxTime_),
                                                                               task(task_)
        {
        }

        uint64_t time;
        uint64_t maxTime;
        function<void()> task;
    } TaskNode;

    list<TaskNode> taskList;
    list<function<void()>> taskQueue;

    shared_ptr<WorkQueue> w;

    bool bStatus;
    uint64_t timeStamp;
    uint64_t tvS, tvE;
};

}; // namespace lmc
#endif

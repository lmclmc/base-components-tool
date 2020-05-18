#ifndef LTIMER_H_
#define LTIMER_H_

#include <memory>
#include <functional>
#include <list>

#include "threadpool/workqueue.h"
#include "single/spinmutex.hpp"

namespace lmc
{
class LTimer
{
public:
    LTimer();

    ~LTimer() = default;

    uint64_t setTimer(uint64_t, const std::function<void()> &, int64_t count = -1);
    void removeTimer(uint64_t uuid);

    void startTimer();
    void stopTimer();
    void clearTimer();

private:
    void task();

private:
    typedef struct TaskNode_
    {
        TaskNode_(uint64_t time_,
                  uint64_t maxTime_,
                  const std::function<void()> &task_,
                  int64_t count_,
                  uint64_t uuid_) : time(time_),
                                    maxTime(maxTime_),
                                    task(task_),
                                    count(count_),
                                    uuid(uuid_)
        {
        }

        uint64_t time;
        uint64_t maxTime;
        uint64_t count;
        uint64_t uuid;
        std::function<void()> task;
    } TaskNode;

    std::list<TaskNode> taskList;
    std::list<function<void()>> taskQueue;

    std::shared_ptr<lmc::WorkQueue> w;

    bool bStatus;
    uint64_t timeStamp;
    uint64_t tvS, tvE;

    lmc::SpinMutex mutex;
};
}; // namespace lmc

#endif

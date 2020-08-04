#ifndef LTIMER_H_
#define LTIMER_H_

#include <memory>
#include <functional>
#include <list>

#include "threadpool/workqueue.h"
#include "single/spinmutex.hpp"

using namespace std;
using namespace lmc;

namespace lmc
{

class LTimer
{
public:
    LTimer();

    ~LTimer() = default;

    /**
     * @brief setTimer 设置定时器
     * @param time 隔多长时间触发一次 单位毫秒
     * @param f 需要执行的任务
     * @param count 调用次数，-1表示调用无限次
     * @return 返回uuid，就靠这个uuid移除该任务
     */
    uint64_t setTimer(uint64_t time, const function<void()> &f,
                      int64_t count = -1);

    /**
     * @brief removeTimer 根据uuid移除定时器任务
     * @param uuid      设置uuid参数
     */
    void removeTimer(uint64_t uuid);

    /**
     * @brief startTimer 开启定时器
     */
    void startTimer();

    /**
     * @brief stopTimer 暂停定时器
     */
    void stopTimer();

    /**
     * @brief clearTimer 清除定时器任务
     */
    void clearTimer();

private:
    void task();

private:
    typedef struct TaskNode_
    {
        TaskNode_(uint64_t time_,
                  uint64_t maxTime_,
                  const function<void()> &task_,
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
        function<void()> task;
    } TaskNode;

    list<TaskNode> taskList;
    list<function<void()>> taskQueue;

    shared_ptr<WorkQueue> w;

    bool bStatus;
    uint64_t timeStamp;
    uint64_t tvS, tvE;

    SpinMutex mutex;
};

}; // namespace lmc
#endif

/*
 * @Description: 单线程定时器，支持多次定时器任务的添加，删除。
 * @Author: luomincheng
 * @Date: 2023-04-16 15:28:55 
 * @Last Modified time: 2023-04-16 15:37:29
 */

#ifndef LTIMER_H_
#define LTIMER_H_

#include <memory>
#include <functional>
#include <list>

#include "threadpool/workqueue.h"
#include "util/spinmutex.hpp"

using namespace std;
using namespace lmc;

namespace lmc
{
class LTimer
{
public:
    LTimer();

    ~LTimer();

    /**
     * @brief setTimer 设置定时器
     * @param time 隔多长时间触发一次 单位毫秒
     * @param f 需要执行的任务
     * @param count 调用次数，-1表示调用无限次
     * @return 返回uuid，就靠这个uuid移除该任务
     */
    uint64_t setTimer(int64_t time, const function<void()> &f,
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

        int64_t time;     //任务准备时间，该时间不断更新，当准备时间为0时，执行任务
        int64_t maxTime;  //任务超时时间
        int64_t count;    //任务执行次数，-1代表无限执行
        uint64_t uuid;    //任务uuid
        function<void()> task;
    } TaskNode;

    list<TaskNode> taskList;
    list<function<void()>> taskQueue;

    shared_ptr<WorkQueue> w;

    bool bStatus;  //是否开启定时器
    int64_t timeStamp; //时间戳
    int64_t tmpTimeStamp; //临时时间戳，不断更新，每次执行一轮任务后，
                          //更新为最近的任务的时间。
    int64_t tvS, tvE;

    SpinMutex mutex;
};

}; // namespace lmc
#endif

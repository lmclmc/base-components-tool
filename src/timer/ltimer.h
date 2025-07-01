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

using namespace std;

namespace lmc {
class LTimer {
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
    class Impl;
    std::unique_ptr<Impl> pImpl;   
};
}; // namespace lmc

#endif
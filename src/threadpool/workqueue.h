/*
 * @Description: 单线程线程池，支持任意类型，任意参数数量，参数类型的任务的添加
 * @Author: luomincheng
 * @Date: 2023-04-16 15:28:55 
 * @Last Modified by: luomincheng
 * @Last Modified time: 2023-04-16 15:37:16
 */

#ifndef WORKQUEUE_H_
#define WORKQUEUE_H_

#include "lthread.h"
#include "util/spinmutex.hpp"

#include <future>
#include <iostream>
#include <functional>
#include <queue>
#include <mutex>

using namespace std;

namespace lmc {
enum class MutexType: unsigned char
{
    None,
    Spin,
    Mutex,
};

class SMutex
{
public:
    SMutex() = default;
    void lock();
    void unlock();
    void setMutexType(MutexType m);

private:
    SpinMutex mSpinMutex;
    mutex mMutex;
    MutexType mMutexType;
};

class WorkQueue final : public Thread{
public:
    WorkQueue(MutexType m);
    ~WorkQueue();

    /**
     * @brief 添加任务
     * @param 第一个参数为任务函数指针，可以使匿名函数，后面的参数均为前面函数的参数
     * @return 用户可以控制返回值是否阻塞，当任务执行完成后，
     * 返回值都将变成非阻塞状态，并且返回任务的最终返回值。
     */
    template <typename F, typename ...Args>
    auto addTask(F &&f, Args &&...args) throw() ->
    future<typename result_of<F(Args...)>::type>
    {
        using returnType = typename result_of<F(Args...)>::type;
        auto task = make_shared<packaged_task<returnType()>>(bind(
                                forward<F>(f), forward<Args>(args)...));
        future<returnType> returnRes = task.get()->get_future();

        mutex.lock();
        workqueue.emplace([task]{(*task)();});
        mutex.unlock();

        start();
        return returnRes;
    }

protected:
    void run() override;

private:
    queue<function<void()>> workqueue;
    SMutex mutex;
};

}

#endif

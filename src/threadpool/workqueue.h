#ifndef WORKQUEUE_H_
#define WORKQUEUE_H_

#include "thread/lthread.h"
#include "single/spinmutex.hpp"

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

    template <typename F, typename ...Args>
    auto addTask(F &&f, Args &&...args) throw() ->
    future<typename result_of<F(Args...)>::type>{
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

#ifndef WORKQUEUE_H_
#define WORKQUEUE_H_

#include "thread/lthread.h"

#include <future>
#include <iostream>
#include <functional>
#include <queue>

using namespace std;

namespace lmc {

class WorkQueue : public Thread{
public:
    WorkQueue();
    ~WorkQueue();

    template <typename F, typename... Args>
    auto addTask(F&& f, Args&&... args) throw() ->
    future<typename result_of<F(Args...)>::type>{
        using returnType = typename result_of<F(Args...)>::type;
        auto task = make_shared<packaged_task<returnType()>>(bind(
                                forward<F>(f), forward<Args>(args)...));
        future<returnType> returnRes = task.get()->get_future();

        workqueue.emplace([task]{(*task)();});

        start();
        return returnRes;
    }

protected:
    void run() override;

private:
    queue<function<void()>> workqueue;
};

}

#endif

#ifndef WORKQUEUE_H_
#define WORKQUEUE_H_

#include "lthread.h"
#include "yqueue/ypipe.hpp"

#include <future>
#include <iostream>
#include <functional>

using namespace zmq;
using namespace std;

namespace lmc {
    class WorkQueue : public Thread{
    public:
        typedef ypipe_t<shared_ptr<function<void()>>, SIZE> workqueue;

        WorkQueue();
        ~WorkQueue();

        template <typename F, typename... Args>
        auto addTask(F&& f, Args&&... args) throw() ->
        future<typename result_of<F(Args...)>::type>{
            using returnType = typename result_of<F(Args...)>::type;
            auto task = make_shared<packaged_task<returnType()>>(bind(forward<F>(f), forward<Args>(args)...));
            future<returnType> returnRes = task.get()->get_future();

            queue->write(make_shared<function<void()>>([task]{(*task)();}), false);
            queue->flush();

            start();
            return returnRes;
        }

    protected:
        void run() override;

    private:
        shared_ptr<workqueue> queue;
    };
}

#endif

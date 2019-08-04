#ifndef THREAD_H
#define THREAD_H

#include <iostream>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <future>

#include "yqueue/ypipe.hpp"

using namespace std;
using namespace zmq;

namespace lmc{
    class Thread
    {
    public:
        Thread();
        ~Thread();

        void start();

    protected:
        virtual void run();

    private:
        void destory();

    private:
        condition_variable c;
        atomic<long> cStatus;
        atomic<bool> bStop;
        thread t;
    };

    class WorkQueue : public Thread{
    public:
        typedef ypipe_t<shared_ptr<function<void()>>, 10000> workqueue;

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

            condition.notify_one();
            return returnRes;
        }

    protected:
        void run();

    private:
        condition_variable condition;
        shared_ptr<workqueue> queue;
    };

    class testThread : public Thread{
    public:
        testThread();

        void test();

    protected:
        void run();
    };
}

#endif // THREAD_H

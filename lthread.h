#ifndef THREAD_H
#define THREAD_H

#include <condition_variable>
#include <atomic>
#include <thread>

#define SIZE (10000)

using namespace std;

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
}

#endif // THREAD_H

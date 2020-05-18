#ifndef THREAD_H
#define THREAD_H

#include <condition_variable>
#include <atomic>
#include <thread>

#define SIZE (10000)

namespace lmc{
    class Thread
    {
    public:
        Thread();
        ~Thread();

    protected:
	void start();
        virtual void run();

    private:
        void destory();

    private:
        std::condition_variable c;
        std::atomic<long> cStatus;
        std::atomic<bool> bStop;
        std::thread t;
    };
}

#endif // THREAD_H

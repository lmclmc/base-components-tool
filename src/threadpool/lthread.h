/*
 * @Description: 线程类，允许被继承。
 * @Author: luomincheng
 * @Date: 2023-04-16 15:28:55 
 * @Last Modified by: mikey.zhaopeng
 * @Last Modified time: 2023-04-16 15:38:04
 */

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
    virtual ~Thread();

protected:
    void start();
    virtual void run() = 0;

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

/*
 * @Description: 线程类，允许被继承。
 * @Author: luomincheng
 * @Date: 2023-04-16 15:28:55 
 */

#ifndef THREAD_H
#define THREAD_H

#include <memory>

namespace lmc {
class Thread {
public:
    Thread();
    virtual ~Thread();

protected:
    void start();
    virtual void run() = 0;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;   
};
}

#endif // THREAD_H
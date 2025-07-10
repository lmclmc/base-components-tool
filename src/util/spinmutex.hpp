#ifndef SPINMUTEX_HPP_
#define SPINMUTEX_HPP_

#include <atomic>

namespace lmc {
class SpinMutex {
public:
    SpinMutex() : flag_(false) {}

    inline void lock() {
        bool expect = false;
        while (!flag_.compare_exchange_weak(expect, true)) {
            expect = false;
        }
    }

    inline void unlock() {
        flag_.store(false);
    }

private:
    std::atomic<bool> flag_;
};
}

#endif
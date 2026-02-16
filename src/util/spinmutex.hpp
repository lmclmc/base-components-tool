#ifndef SPINMUTEX_HPP_
#define SPINMUTEX_HPP_

#include <atomic>

namespace lmc {
class SpinMutex {
public:
    SpinMutex() : flag_(false) {}

    inline void lock() {
        bool expect = false;
        while (!flag_.compare_exchange_weak(expect, true,
                                            std::memory_order_acquire,
                                            std::memory_order_relaxed)) {
            expect = false;
        }
    }

    inline void unlock() {
        flag_.store(false, std::memory_order_release);
    }

private:
    std::atomic<bool> flag_;
};
}

#endif

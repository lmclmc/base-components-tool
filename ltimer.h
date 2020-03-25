#ifndef LTIMER_H_
#define LTIMER_H_

#include <memory>
#include <functional>

#include "workqueue.h"

using namespace std;
using namespace lmc;

class LTimer
{
public:
    LTimer();

    ~LTimer() = default;

    void setTimer(uint64_t time, function<void()> f);

    void startTimer();
    void stopTimer();
    void clearTimer();

private:
    std::shared_ptr<WorkQueue> w;
    function<void()> mFun;
    uint64_t mTime;
    bool bStatus;
};

#endif

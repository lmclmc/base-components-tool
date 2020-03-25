#include <sys/time.h>
#include <unistd.h>

#include "ltimer.h"

using namespace std;

LTimer::LTimer() :
    bStatus(false),
    w(make_shared<WorkQueue>())
{}

void LTimer::setTimer(uint64_t time, function<void()> f)
{
    mTime = time*1000;
    mFun = f;
}

void LTimer::startTimer()
{
    bStatus = true;

    w->addTask([this]{
        struct timeval tvS, tvE;
        gettimeofday(&tvS, NULL);
        uint64_t timestamp;
        while (this->bStatus)
        {
            gettimeofday(&tvE, NULL);
            timestamp = (tvE.tv_sec-tvS.tv_sec)*1000000+tvE.tv_usec-tvS.tv_usec;
            ::usleep(this->mTime-timestamp);
            gettimeofday(&tvS, NULL);
            this->mFun();    
        }
    });
}

void LTimer::stopTimer()
{
    bStatus = false;
}
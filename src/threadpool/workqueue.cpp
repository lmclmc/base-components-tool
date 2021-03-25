#include "workqueue.h"

using namespace lmc;

WorkQueue::WorkQueue()
{
    start();
}

WorkQueue::~WorkQueue()
{

}

void WorkQueue::run()
{
    if (workqueue.empty()) return;

    function<void()> f = move(workqueue.front());
    workqueue.pop();
    f();
}

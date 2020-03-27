#include "workqueue.h"

using namespace lmc;

WorkQueue::WorkQueue() :
    queue(make_shared<workqueue>())
{
    start();
}

WorkQueue::~WorkQueue()
{

}

void WorkQueue::run()
{
    shared_ptr<function<void()>> f;

    if (queue->read(&f)) (*f)();
    else cout << "err" << endl;
}

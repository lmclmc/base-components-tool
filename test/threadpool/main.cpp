#include <iostream>
#include <unistd.h>

#include "threadpool/workqueue.h"
#include "timer/ltimer.h"
#include "single/single.hpp"

using namespace std;
using namespace lmc;

void asd(){
    static int num = 0;
    if (num%100000 == 0) cout << num << endl;
        num++;

}

int main()
{
    
     WorkQueue *a = TypeSingle<WorkQueue>::getInstance();
    while (1)
    {
        a->addTask(asd);
    }
    return 0;
}

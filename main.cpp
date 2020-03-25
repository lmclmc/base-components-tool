#include <iostream>
#include "workqueue.h"
#include "ltimer.h"
#include <unistd.h>
using namespace std;
using namespace lmc;

void asd(){
    static int num = 0;
   //     cout << num << endl;
    if (num%1000000 == 0) cout << num << endl;
        num++;

}

void qwe(WorkQueue& a){
  //  for (int i=0;i<2000000;i++){
       // ::usleep(100);
        a.addTask(asd);
   // }
}

void zxc(WorkQueue& a){
    for (int i=0;i<2000000000;i++){
        a.addTask(asd);
    }
}

int main()
{
    // WorkQueue a;

    // while (1){
    //     for (int i=0;i<2000000000;i++){

    //         a.addTask(asd);
    //     }
    //     cout << "Hello World!\n\n\n" << endl;
    //     getchar();

    // }

    LTimer t;
    t.setTimer(500, []{
        cout << "qwe" << endl;
    });

    t.startTimer();

    while (1)
    {
        ::usleep(1000000);
        cout << "asd" << endl;
    }
    return 0;
}

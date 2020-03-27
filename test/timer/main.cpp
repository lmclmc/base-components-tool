#include <iostream>
#include "threadpool/workqueue.h"
#include "timer/ltimer.h"
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
#include <map>
#define INSERT(num) m.insert(std::pair<int, string>(num, #num))
#include <queue>
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
    t.setTimer(20, []{
        static int a = 0;
        a++;
        cout << "1 ==" << a << endl;
    });

    t.setTimer(1000, []{
        static int a = 0;
        a++;
        cout << "222 ==" << a << endl;
    });

    t.setTimer(300, []{
        static int a = 0;
        a++;
        cout << "33333 ==" <<  a << endl;
    });

    t.setTimer(700, []{
        static int a = 0;
        a++;
        cout << "4444444 ==" <<  a << endl;
    });

    t.setTimer(1500, []{
        static int a = 0;
        a++;
        cout << "6666666666666 ==" <<  a << endl;
    });

    t.startTimer();
    int a = 0;
    while (1)
    {
         ::usleep(3000000);
         a++;
         cout << "asd" << endl;
    }

    return 0;
}

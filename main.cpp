#include <iostream>
#include "workqueue.h"
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
    WorkQueue a;

    while (1){
        for (int i=0;i<2000000000;i++){

            a.addTask(asd);
        }
        cout << "Hello World!\n\n\n" << endl;
        getchar();

    }

    return 0;
}

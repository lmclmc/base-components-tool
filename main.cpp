#include <iostream>
#include "thread.h"
using namespace std;
using namespace lmc;

int main()
{
    WorkQueue a;

    while (1){
        for (int i=0; i<10000; i++){
            a.addTask([](int a){
                cout << a << "         Hello fqwegqrg! ==" << endl;
            }, i);
        }
        cout << "Hello World!\n\n\n" << endl;
        getchar();

    }

    return 0;
}

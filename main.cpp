#include <iostream>
#include "thread.h"
using namespace std;
using namespace lmc;

int main()
{
    testThread a;

    while (1){
        for (int i=0; i<100; i++){
            a.test();
        }
        cout << "Hello World!\n\n\n" << endl;
        getchar();
        a.test();
    }

    return 0;
}

#include <iostream>
#include "thread.h"
using namespace std;
using namespace lmc;

int main()
{
    testThread a;

    while (1){
        getchar();
        a.test();
    }
    cout << "Hello World!" << endl;
    return 0;
}

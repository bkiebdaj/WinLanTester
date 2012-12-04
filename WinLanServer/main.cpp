#include<iostream>
#include"server.h"
using namespace std;

int main()
{
    Server serw;
    serw.start(100);
    serw.clean();
    return 0;
}

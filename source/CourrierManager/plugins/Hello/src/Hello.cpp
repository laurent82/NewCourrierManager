#include "Hello.h"

#include <iostream>
using namespace std;

Hello::Hello() :
    CAbstractFilter()
{
}

void Hello::execute()
{
    printHello();
}

void Hello::printHello()
{
    cout << "Hello world" << endl;
}

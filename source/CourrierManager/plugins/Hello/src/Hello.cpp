#include "Hello.h"

#include <iostream>
using namespace std;

Hello::Hello(const QString &title) :
    CAbstractFilter(title)
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

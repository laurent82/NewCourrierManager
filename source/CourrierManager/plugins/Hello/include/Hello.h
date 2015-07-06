#ifndef HELLO_H
#define HELLO_H

#include <CAbstractFilter.h>

class Hello : public CAbstractFilter
{
public:
    Hello(const QString& title);
    virtual void execute();

private:
    void printHello();
};

#endif 

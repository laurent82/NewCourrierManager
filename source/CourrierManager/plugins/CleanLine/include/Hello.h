#ifndef HELLO_H
#define HELLO_H

#include <QtCore/QtPlugin>

class Hello
{
	public:
	Hello() {}
	static void printHello();
};

Q_DECLARE_INTERFACE(Hello, "Hello")

#endif 

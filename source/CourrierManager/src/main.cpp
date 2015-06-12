#include <QtWidgets/QApplication>
#include "CControler.h"

#define USE_QT5 1

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CControler w;
    return a.exec();
}

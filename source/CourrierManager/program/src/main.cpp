#include <QtWidgets/QApplication>
#include "CControler.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("LaurentEngelsSuite");
    QCoreApplication::setApplicationName("CourrierManager");

    CControler w;
    return a.exec();
}

#include <QtWidgets/QApplication>
#include <QFile>
#include <QDebug>
#include "CControler.h"
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("LaurentEngelsSuite");
    QCoreApplication::setApplicationName("CourrierManager");
    // Loading stylesheet
    QFile file_sheet(":/stylesheet.qss");
    if ( file_sheet.open(QFile::ReadOnly) )
    {
        QString full_styleSheet = QLatin1String(file_sheet.readAll());
        app.setStyleSheet( full_styleSheet );
    }
    else
    {
        qWarning() << "Stylesheet introuvable...";
    }
    CControler w;
    return app.exec();
}

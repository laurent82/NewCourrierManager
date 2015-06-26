#ifndef HELLOPLUGIN_H
#define HELLOPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <CAbstractPlugin.h>

class HelloPlugin : public QObject
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.courriermanager.HelloPlugin")
    Q_INTERFACES(CAbstractPlugin)

public:
    HelloPlugin();

    virtual CAbstractFilter *createFilter();

};

#endif

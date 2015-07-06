#ifndef CABSTRACTPLUGIN_H
#define CABSTRACTPLUGIN_H

#include <QtPlugin>
#include <QObject>

class CAbstractFilter;

class CAbstractPlugin : public QObject
{
    Q_OBJECT
public:
    virtual CAbstractFilter* createFilter() = 0;

protected:
    CAbstractPlugin();
};

Q_DECLARE_INTERFACE(CAbstractPlugin, "CAbstractPlugin")


#endif

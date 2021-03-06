#ifndef CPLUGINLOADER_H
#define CPLUGINLOADER_H

#include <QList>
#include <memory>

class CAbstractPlugin;

class CPluginLoader
{
public:
    CPluginLoader();
    ~CPluginLoader();

    QList<CAbstractPlugin*> getList() const;

private:
    void init();
    QList<CAbstractPlugin *> m_list;

};

#endif

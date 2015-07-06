#ifndef CPLUGINLOADER_H
#define CPLUGINLOADER_H

#include <QList>

class CAbstractFilter;

class CPluginLoader
{
public:
    CPluginLoader();
    ~CPluginLoader();

    QList<CAbstractFilter*> getList() const;

private:
    void init();
    QList<CAbstractFilter*> m_list;

};

#endif

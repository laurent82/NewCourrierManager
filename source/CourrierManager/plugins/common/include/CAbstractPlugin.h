#ifndef CABSTRACTPLUGIN_H
#define CABSTRACTPLUGIN_H

#include <QtPlugin>
class CAbstractFilter;

class CAbstractPlugin
{
public:
    virtual CAbstractFilter* createFilter() = 0;

protected:
    CAbstractPlugin();

};

#endif

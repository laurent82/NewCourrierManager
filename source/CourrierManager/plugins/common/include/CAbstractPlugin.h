#ifndef CABSTRACTPLUGIN_H
#define CABSTRACTPLUGIN_H

#include <QString>

class CAbstractPlugin 
{
public:
    CAbstractPlugin();
    void setName( QString name);
    QString getName() const;
    void setActive(bool);
    bool isActive() const;
protected:
    QString m_name;
    bool m_active;
};

#endif

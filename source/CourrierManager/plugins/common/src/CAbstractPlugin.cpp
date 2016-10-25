#include "CAbstractPlugin.h"

CAbstractPlugin::CAbstractPlugin()
    : m_active(true)
{
}

bool CAbstractPlugin::isActive() const
{
    return m_active;
}


void CAbstractPlugin::setName(QString name)
{
    m_name = name;
}

void CAbstractPlugin::setActive(bool active)
{
    m_active = active;
}



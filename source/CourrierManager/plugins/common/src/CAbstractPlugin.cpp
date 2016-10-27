#include "CAbstractPlugin.h"

CAbstractPlugin::CAbstractPlugin()
    : m_active(true)
{
}

CAbstractPlugin::~CAbstractPlugin()
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

QString CAbstractPlugin::getName() const
{
    return m_name;
}




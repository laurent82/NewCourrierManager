#include "CPatient.h"

CPatient::CPatient()
{
}

void CPatient::configure(const QString &key, const QVariant &value)
{
    m_parameters[key] = value;
}

void CPatient::clear()
{
    m_parameters.clear();
}

QVariant CPatient::getParameter(const QString &key) const
{
    if (m_parameters.contains(key))
        return m_parameters[key];
    return QVariant();
}

void CPatient::incrementPage()
{
    if (!m_parameters.contains("page"))
        return;
    int pageNumber = m_parameters["page"].toInt();
    m_parameters["page"] = ++pageNumber;
}

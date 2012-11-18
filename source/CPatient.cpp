#include "CPatient.h"

CPatient* CPatient::m_instance = 0;

CPatient::CPatient()
{
    m_parameters["patient_table"] = false;
}

CPatient* CPatient::instance()
{
    if (!m_instance){
        m_instance = new CPatient();
    }
    return m_instance;
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
    if (!m_parameters.contains("patient_page"))
        return;
    int pageNumber = m_parameters["patient_page"].toInt();
    m_parameters["patient_page"] = ++pageNumber;
}

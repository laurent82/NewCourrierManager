#include "CAbstractFilter.h"

CAbstractFilter::CAbstractFilter()
{

}

void CAbstractFilter::setInput(const QVariant &input)
{
    m_inputMap["default"] = input;
}

void CAbstractFilter::setInput(const QVariant &input, const QString &key)
{
    m_inputMap[key] = input;
}

QVariant CAbstractFilter::getOutput() const
{
    return m_output;
}

QVariant CAbstractFilter::getInput() const
{
    return m_inputMap["default"];
}

QVariant CAbstractFilter::getInput(const QString& key) const
{
    return m_inputMap[key];
}






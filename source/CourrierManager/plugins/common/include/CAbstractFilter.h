#ifndef CABSTRACTFILTER_H
#define CABSTRACTFILTER_H

#include <QVariantMap>

class CAbstractFilter
{
public:
    CAbstractFilter();

    void setInput(const QVariant& input);
    void setInput(const QVariant& input, const QString& key);
    virtual void execute() = 0;
    QVariant getOutput() const;

protected:
    QVariant getInput() const;
    QVariant getInput(const QString& key)  const;

    QVariantMap m_inputMap;
    QVariant m_output;

};

#endif

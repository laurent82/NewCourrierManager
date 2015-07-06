#ifndef CABSTRACTFILTER_H
#define CABSTRACTFILTER_H

#include <QVariantMap>
#include <QString>

class CAbstractFilter
{
public:
    CAbstractFilter(const QString name);

    void setInput(const QVariant& input);
    void setInput(const QVariant& input, const QString& key);
    virtual void execute() = 0;
    QVariant getOutput() const;

    bool isActive() const;
    void setActive(bool active);

    bool isOutputCreated() const;

protected:
    QVariant getInput() const;
    QVariant getInput(const QString& key)  const;

    void setOutputCreated(bool outputCreated);

    QVariantMap m_inputMap;
    QVariant m_output;

    bool m_active;
    bool m_outputCreated;

    QString m_name;

};

#endif

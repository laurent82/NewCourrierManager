#ifndef CACTION_H
#define CACTION_H

#include <QVariant>
#include <QObject>

class CAction : public QObject
{
    Q_OBJECT
public:
    static int getLastActionId();
    int getId();
    virtual QString getActionType() const = 0;
    virtual bool undo() = 0;
    void setPreviousState(QVariant state);
    void setCurrentState(QVariant state);

    void  setValid(bool valid);
    bool isValid() const;

protected:
    CAction();
    int m_id;
    static int idcount;
    QVariant m_previousState;
    QVariant m_currentState;
    bool m_valid;

};

#endif

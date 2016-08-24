#ifndef CACTIONMANAGER_H
#define CACTIONMANAGER_H

#include <QVector>
#include <QObject>

class CAction;

class CActionManager : public QObject
{
    Q_OBJECT
public:
    static CActionManager* instance();
    void push(CAction* action);
    CAction* findAction(int id);
    CAction* getLastAction();
    bool cancelActionById(int id);
    bool cancelAction(CAction* action);
protected:
    CActionManager();

protected slots:
    void onCancelActionDone(QString name);

signals:
    void actionUndone(int id);

private:
    static CActionManager* m_instance;
    QVector<CAction*> m_memory;
};

#endif

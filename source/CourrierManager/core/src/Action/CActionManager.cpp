#include "Action/CActionManager.h"
#include "Action/CCancelAction.h"
#include "Action/CAction.h"

#include <QDebug>
#include <QFile>

CActionManager* CActionManager::m_instance = 0;

CActionManager *CActionManager::instance()
{
    if (m_instance == 0)
    {
        m_instance = new CActionManager();
    }
    return m_instance;
}

void CActionManager::push(CAction *action)
{
    // Cancel action?
    CCancelAction* cancel = dynamic_cast<CCancelAction*>(action);
    if (cancel) {
        connect (cancel, SIGNAL(undoDone(QString)), this, SLOT(onCancelActionDone(QString)));
    }
    m_memory.push_back(action);
}

CAction* CActionManager::findAction(int id)
{
    for(int i = 0; i < m_memory.count(); ++i)
    {
        if (m_memory[i]->isValid() && m_memory[i]->getId() == id)
        {
            CAction* action = m_memory[i];
            m_memory.remove(i);
            return action;
        }
    }
    return 0;
}

CAction *CActionManager::getLastAction()
{
    return m_memory.last();
}

bool CActionManager::cancelActionById(int id)
{
    CAction* currentAction = findAction(id);
    if (currentAction)
    {
        return cancelAction(currentAction);
    } else {
        return false;
    }
}

bool CActionManager::cancelAction(CAction *action)
{
    if (action) {
        bool result = action->undo();
        delete action;
        return result;
    } else {
        return false;
    }

}

CActionManager::CActionManager() :
    QObject()
{
}

void CActionManager::onCancelActionDone(QString name)
{
    // Research all the action with the same PDF
    foreach (CAction* action, m_memory) {
        CCancelAction* cancel = dynamic_cast<CCancelAction*>(action);
        if (cancel && cancel->isValid()) {
            if (name == cancel->getPdfName(0))
            {
                cancel->setSingleCancel(true);
                if (cancel->undo()) {
                    emit actionUndone(cancel->getId());
                    cancel->setValid(false);
                }

            }
        }
    }

    // Clean the vector
    for (int i = 0; i < m_memory.size(); ++i) {
        if (m_memory[i]->isValid() == false) {
            m_memory.remove(i);
            --i;
        }
    }
}

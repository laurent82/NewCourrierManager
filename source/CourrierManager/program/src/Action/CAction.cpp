#include "Action/CAction.h"

int CAction::idcount = 0;

CAction::CAction()
{
    m_id = ++idcount;
    m_valid = true;
}

int CAction::getLastActionId()
{
    return (idcount);
}

int CAction::getId()
{
    return m_id;
}

void CAction::setPreviousState(QVariant state)
{
    m_previousState = state;
}

void CAction::setCurrentState(QVariant state)
{
    m_currentState = state;
}

void CAction::setValid(bool valid)
{
    m_valid = valid;
}

bool CAction::isValid() const
{
    return m_valid;
}

#include "CControler.h"
#include "CView.h"
#include "CGestion.h"

CControler::CControler() : QObject()
{
    m_view = new CView();
    m_gestion = new CGestion();

    connect(m_view, SIGNAL(criticalError()), this, SLOT(onCriticalError()));
    connect(m_gestion, SIGNAL(errorOccur(int)), m_view, SLOT(displayError(int)));
    connect(m_view, SIGNAL(btnSearchClicked()), m_gestion, SLOT(onSearchClicked()));
    connect(m_gestion, SIGNAL(setFile(QString)), m_view, SLOT(onSetFile(QString)));
//    connect(m_view, SIGNAL(btnSendwClicked()), m_gestion, SLOT(onSendClicked()));
//    connect(m_view, SIGNAL(btnSamePatientClicked()), m_gestion, SLOT(onSamePatientClicked()));
//    connect(m_view, SIGNAL(btnValidateClicked()), m_gestion, SLOT(onValidateClicked()));
//    connect(m_view, SIGNAL(btnSkipClicked()), m_gestion, SLOT(onSkipClicked()));
//    connect(m_view, SIGNAL(btnDeleteClicked()), m_gestion, SLOT(onDeleteClicked()));

    m_gestion->loadConfigFile();
    m_view->show();
}

void CControler::onCriticalError()
{
    exit(0);
}

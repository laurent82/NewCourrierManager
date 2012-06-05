#include "CControler.h"
#include "CView.h"
#include "CGestion.h"

CControler::CControler() : QObject()
{
    m_view = new CView();
    m_gestion = new CGestion();

    connect(m_view, SIGNAL(btnValidateClicked()), m_gestion, SLOT(onValidate()));


    connect(m_view, SIGNAL(criticalError()), this, SLOT(onCriticalError()));
    connect(m_gestion, SIGNAL(errorOccur(int)), m_view, SLOT(displayError(int)));
    connect(m_view, SIGNAL(btnSearchClicked()), m_gestion, SLOT(onSearch()));
    connect(m_gestion, SIGNAL(setFile(QString)), m_view, SLOT(onSetFile(QString)));
    connect(m_view, SIGNAL(deleteFile()), m_gestion, SLOT(onDeleteFile()));
    connect(m_gestion, SIGNAL(sentRefreshRemaining(int,int)), m_view, SLOT(onRefreshRemaining(int,int)));

//    connect(m_view, SIGNAL(btnSendwClicked()), m_gestion, SLOT(onSendClicked()));
//    connect(m_view, SIGNAL(btnSamePatientClicked()), m_gestion, SLOT(onSamePatientClicked()));
//    connect(m_view, SIGNAL(btnSkipClicked()), m_gestion, SLOT(onSkipClicked()));

    m_gestion->loadConfigFile();
    m_view->show();
}

void CControler::onCriticalError()
{
//    exit(0);
}

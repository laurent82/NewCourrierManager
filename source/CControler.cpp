#include "CControler.h"
#include "CView.h"
#include "CFileManager.h"
#include "CConfigFrame.h"
#include "CError.h"

#include <QDebug>
#include <QApplication>
CControler::CControler() : QObject()
{
    m_view = new CView();
    m_fileManager = new CFileManager();

    //connect(m_view, SIGNAL(btnValidateClicked()), m_fileManager, SLOT(onValidate()));
    connect(m_view, SIGNAL(sendCommand(QString)), m_fileManager, SLOT(onCommandReceived(QString)));
    connect(m_view, SIGNAL(btnConfigurationClicked()), this, SLOT(showConfigDialog()));
    connect(m_fileManager, SIGNAL(sendInfo(QString,QVariant)), m_view, SLOT(onInfoReceived(QString, QVariant)));
    // Gestion des erreurs:
    connect(m_fileManager, SIGNAL(errorOccur(int)), this, SLOT(onError(int)));

    m_fileManager->loadConfigFile();
    m_view->show();

    // To be confirmed...
//    connect(m_view, SIGNAL(criticalError()), this, SLOT(onCriticalError()));
//    connect(m_gestion, SIGNAL(errorOccur(int)), m_view, SLOT(displayError(int)));
//    connect(m_view, SIGNAL(btnSearchClicked()), m_gestion, SLOT(onSearch()));
//    connect(m_gestion, SIGNAL(setFile(QString)), m_view, SLOT(onSetFile(QString)));
//    connect(m_view, SIGNAL(deleteFile()), m_gestion, SLOT(onDeleteFile()));
//    connect(m_gestion, SIGNAL(sentRefreshRemaining(int,int)), m_view, SLOT(onRefreshRemaining(int,int)));

//    connect(m_view, SIGNAL(btnSendwClicked()), m_gestion, SLOT(onSendClicked()));
//    connect(m_view, SIGNAL(btnSamePatientClicked()), m_gestion, SLOT(onSamePatientClicked()));
//    connect(m_view, SIGNAL(btnSkipClicked()), m_gestion, SLOT(onSkipClicked()));

}

void CControler::onError(int errorId)
{
    qDebug() << "Error number : " << errorId;
    switch(errorId) {
        case CError::NOCONFIGFILE:
            showConfigDialog();
        break;
    }
}
void CControler::onCriticalError()
{

//    exit(0);
}

void CControler::showConfigDialog()
{
    CConfigFrame::instance()->show();
}

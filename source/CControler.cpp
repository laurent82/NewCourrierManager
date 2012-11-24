#include "CControler.h"
#include "CView.h"
#include "CFileManager.h"
#include "CConfigFrame.h"
#include "CError.h"

#include <QApplication>
CControler::CControler() : QObject()
{
    m_view = new CView();
    m_fileManager = new CFileManager();

    connect(m_view, SIGNAL(sendCommand(QString)), m_fileManager, SLOT(onCommandReceived(QString)));
    connect(m_view, SIGNAL(btnConfigurationClicked()), this, SLOT(showConfigDialog()));
    connect(m_fileManager, SIGNAL(sendInfo(QString,QVariant)), m_view, SLOT(onInfoReceived(QString, QVariant)));

    // Gestion des erreurs:
    connect(m_fileManager, SIGNAL(errorOccur(int)), this, SLOT(onError(int)));

    m_fileManager->loadConfigFile();
    m_view->show();



}

void CControler::onError(int errorId)
{
    switch(errorId) {
        case CError::NOCONFIGFILE:
            showConfigDialog();
        break;
    default:
        m_view->displayError(errorId);
    }

}
void CControler::onCriticalError()
{
    exit(0);
}

void CControler::showConfigDialog()
{
    CConfigFrame::instance()->show();
}

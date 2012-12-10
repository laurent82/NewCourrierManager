#include "CControler.h"
#include "CView.h"
#include "CFileManager.h"
#include "CConfigFrame.h"
#include "CNetwork.h"
#include "CError.h"

#include <QApplication>
CControler::CControler() : QObject()
{
    m_view = new CView();
    m_fileManager = new CFileManager();
    m_network = new CNetwork();

    connect(m_view, SIGNAL(sendCommand(QString)), m_fileManager, SLOT(onCommandReceived(QString)));
    connect(m_view, SIGNAL(sendCommand(QString)), this, SLOT(onCommandReceived(QString)));
    connect(m_view, SIGNAL(btnConfigurationClicked()), this, SLOT(showConfigDialog()));
    connect(m_fileManager, SIGNAL(sendInfo(QString,QVariant)), m_view, SLOT(onInfoReceived(QString, QVariant)));
    connect(this, SIGNAL(errorOccur(int)), m_view, SLOT(displayError(int)));

    connect(m_network, SIGNAL(connectedToHost()), m_view, SLOT(onConnectedToHost()));
    connect(m_network, SIGNAL(disconnectedToHost()), m_view, SLOT(onDisconnectedToHost()));

    // Gestion des erreurs:
    connect(m_fileManager, SIGNAL(errorOccur(int)), this, SLOT(onError(int)));

    m_fileManager->loadConfigFile();
    m_view->show();
}

CControler::~CControler()
{
    delete m_view;
    delete m_fileManager;
    delete m_network;
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

void CControler::onCommandReceived(QString command)
{
    qDebug() << "Commande reçue: " << command;
    if (command.compare("connect", Qt::CaseInsensitive) == 0) {
        m_network->connectToServer();
    }

    if (command.compare("send", Qt::CaseInsensitive) == 0) {
        // Demande de la liste des fichier à envoyer
        if (!m_fileManager) {
            return;
        }
        QStringList list  = m_fileManager->getFilesToSend();

        // Transmettre cette liste au manager réseau
        if (m_network->isConnected()) {
            m_network->sendList(list);
        } else {
            emit errorOccur(CError::NOTCONNECTED);
        }
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

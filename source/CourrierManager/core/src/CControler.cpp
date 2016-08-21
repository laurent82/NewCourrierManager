#include <QtWidgets/QApplication>
#include <QtCore/QDir>

#include "CControler.h"
#include "CView.h"
#include "CFileManager.h"
#include "CConfigFrame.h"
#include "CNetwork.h"
#include "CError.h"

// #include <CPluginLoader.h>
// #include <CAbstractFilter.h>

CControler::CControler() : QObject()
{
    m_view = new CView();
    m_fileManager = new CFileManager();
    m_network = new CNetwork();

    connect(m_view, SIGNAL(sendCommand(QString)), m_fileManager, SLOT(onCommandReceived(QString)));
    connect(m_view, SIGNAL(sendCommand(QString)), this, SLOT(onCommandReceived(QString)));
    connect(m_view, SIGNAL(btnConfigurationClicked()), this, SLOT(showConfigDialog()));
    connect(m_fileManager, SIGNAL(sendInfo(QString,QVariant)), m_view, SLOT(onInfoReceived(QString, QVariant)));
    connect(m_fileManager, SIGNAL(sendInfo(QString,QVariant)), this, SLOT(onInfoReceived(QString, QVariant)));

    connect(this, SIGNAL(errorOccur(int)), m_view, SLOT(displayError(int)));

    connect(m_network, SIGNAL(connectToHost()), m_view, SLOT(onConnectedToHost()));
    connect(m_network, SIGNAL(disconnectedFromHost()), m_view, SLOT(onDisconnectedFromHost()));
    connect(m_network, SIGNAL(allFilesSent()), this, SLOT(onAllFilesSent()));
    connect(m_network, SIGNAL(fileSent()), this, SLOT(onFileSent()));
    connect(m_network, SIGNAL(errorOccur(int)), m_view, SLOT(displayError(int)));

    // Gestion des erreurs:
    connect(m_fileManager, SIGNAL(errorOccur(int)), this, SLOT(onError(int)));

//    // Gestion des plugins
//    CPluginLoader* loader = new CPluginLoader;
//    m_filterList = loader->getList();
//    delete loader;

//    m_fileManager->loadConfigFile(m_ip);
    QSettings settings;
    m_ip = settings.value("serverIP").toString();
    m_network->connectToServer(m_ip);
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
     if (command.compare("connect", Qt::CaseInsensitive) == 0) {
        m_network->connectToServer(m_ip);
    }

    if (command.compare("send", Qt::CaseInsensitive) == 0) {
        // Demande de la liste des fichier � envoyer
        if (!m_fileManager) {
            return;
        }
        QStringList list  = m_fileManager->getFilesToSend();

        // Transmettre cette liste au manager r�seau
        if (list.count() > 0) {
            if (m_network->isConnected()) {
                m_view->setProgressBar(list.count());
                m_network->sendList(list);
            } else {
                emit errorOccur(CError::NOTCONNECTED);
            }
        }  else {
            emit errorOccur(CError::ALLFILESSENT);
        }
    }
}

void CControler::onCriticalError()
{
    exit(0);
}

void CControler::showConfigDialog()
{
    CConfigFrame config;
    config.exec();
}

void CControler::onFileSent()
{
    m_view->updateProgressBar();
}

void CControler::onAllFilesSent()
{
    m_fileManager->onCommandReceived("refresh");
    m_view->displayError(CError::ALLFILESSENT);
}

void CControler::onInfoReceived(QString key, QVariant value)
{
//    if (key == "image") {
//        QVariant target = value;
//        foreach(CAbstractFilter* filter, m_filterList){
//            if (filter->isActive()) {
//                filter->setInput(target);
//                filter->execute();
//                if (filter->isOutputCreated()) {
//                    target = filter->getOutput();
//                }
//            }
//        }
//    }
}

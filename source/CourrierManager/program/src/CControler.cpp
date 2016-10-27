#include <QtWidgets/QApplication>
#include <QtCore/QDir>

#include "CControler.h"
#include "CView.h"
#include "CFileManager.h"
#include "CConfigFrame.h"
#include "Network/CNetwork.h"
#include "CError.h"

#include "Network/CNetworkFTP.h"
#include "Network/CNetworkServer.h"
#include <CPluginLoader.h>

#include "Ocr/COcrManager.h"
#include <CAbstractPlugin.h>

CControler::CControler() 
    : QObject()
{
    m_view.reset( new CView() );
    m_fileManager.reset( new CFileManager() );
    m_network.reset( nullptr );
  
    connect(m_view.get(), SIGNAL(sendCommand(QString)), m_fileManager.get(), SLOT(onCommandReceived(QString)));
    connect(m_view.get(), SIGNAL(sendCommand(QString)), this, SLOT(onCommandReceived(QString)));
    connect(m_view.get(), SIGNAL(btnConfigurationClicked()), this, SLOT(showConfigDialog()));
    connect(m_fileManager.get(), SIGNAL(sendInfo(QString,QVariant)), m_view.get(), SLOT(onInfoReceived(QString, QVariant)));
    connect(m_fileManager.get(), SIGNAL(sendInfo(QString,QVariant)), this, SLOT(onInfoReceived(QString, QVariant)));
    
    connect(this, SIGNAL(errorOccur(int)), m_view.get(), SLOT(displayError(int)));
	 
    // Gestion des erreurs:
    connect(m_fileManager.get(), SIGNAL(errorOccur(int)), this, SLOT(onError(int)));

    initPlugins();    
    initNetwork();
    m_view->updateNetworkMethod();   
    m_view->show();
}

CControler::~CControler()
{
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
         if ( m_network != nullptr ) {
            m_network->connectToServer(m_ip);
         }
         else {
            onError(CError::NOCONFIGUREDNETWORK);
         }
         return;
    }

    if (command.compare("send", Qt::CaseInsensitive) == 0) {
        // Demande de la liste des fichier à envoyer
        if (!m_fileManager) {
            return;
        }
        QStringList list  = m_fileManager->getFilesToSend();

        // Transmettre cette liste au manager réseau
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

        return;
    }

    if ( command.compare("update_config", Qt::CaseInsensitive) == 0)
    {
        initNetwork();
        m_view->updateNetworkMethod();
        return;
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
    onCommandReceived( "update_config" );    
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
    if (key == "image") {
        if ( COcrManager::instance().isActive() )
        {
            QImage image = value.value<QImage>();
            COcrManager::instance().setInput( image );
        }
    }
}

void CControler::initNetwork()
{
	QSettings settings;
	QString network_method = settings.value("network_method").toString();
	if (network_method.compare("ftp", Qt::CaseInsensitive) == 0 )
	{
		m_network.reset( new CNetworkFTP() );

	}
	else if ( network_method.compare("server", Qt::CaseInsensitive) == 0 )
	{
		m_network.reset( new CNetworkServer() );
        m_ip = settings.value("serverIP").toString();
        m_network->connectToServer(m_ip);

	}
    
	// Connect
	connect(m_network.get(), SIGNAL(connectToHost()), m_view.get(), SLOT(onConnectedToHost()));
	connect(m_network.get(), SIGNAL(disconnectedFromHost()), m_view.get(), SLOT(onDisconnectedFromHost()));
	connect(m_network.get(), SIGNAL(allFilesSent()), this, SLOT(onAllFilesSent()));
	connect(m_network.get(), SIGNAL(fileSent()), this, SLOT(onFileSent()));
	connect(m_network.get(), SIGNAL(errorOccur(int)), m_view.get(), SLOT(displayError(int)));
}

void CControler::initPlugins()
{
    std::unique_ptr<CPluginLoader> pluginLoader(new CPluginLoader());
    handlePlugin( pluginLoader->getList() );
}

void CControler::handlePlugin( QList<CAbstractPlugin*> list)
{
    foreach( CAbstractPlugin* plugin, list)
    {
        if (plugin->getName().compare("ocr") == 0)
        {
            handlePlugin_ocr( plugin );
            continue;

        }
    }
}

void CControler::handlePlugin_ocr( CAbstractPlugin* plugin)
{
    COcrManager::instance().setPatientList(m_view->getPatientList());
    connect( &COcrManager::instance(), &COcrManager::sendInfo, m_view.get(), &CView::onInfoReceived );
}


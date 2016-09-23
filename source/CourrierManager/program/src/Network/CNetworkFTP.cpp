#include "Network/CNetworkFTP.h"

// #include <QDataStream>
// #include <QFile>
// #include <QBuffer>

#include "CError.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QSettings>


CNetworkFTPUploader::CNetworkFTPUploader(QUrl& url, const QString& fileName)
    : m_url ( url )
    , m_fileName( fileName )
    , m_reply( nullptr )
{
}

CNetworkFTPUploader::~CNetworkFTPUploader()
{
    delete m_reply;
}

void CNetworkFTPUploader::send()
{
    QFile data(m_fileName);
    if (data.open(QIODevice::ReadOnly))
    {
        m_reply = m_access.put(QNetworkRequest(m_url), &data);
        connect(m_reply, SIGNAL(finished()), this,  SIGNAL(uploadDone() ) );
    } 
}

bool CNetworkFTPUploader::isFinished()
{
    return ( m_reply->isFinished() && 
             m_reply->error() == QNetworkReply::NoError ) ;
}

// --------------------------------------------------------------------------------------------------

CNetworkFTP::CNetworkFTP()
    : CNetwork()
    , m_iter(0)
{
}

CNetworkFTP::~CNetworkFTP()
{
    foreach( CNetworkFTPUploader* uploader, m_uploaderList )
    {
        uploader->deleteLater();
    }
}

void CNetworkFTP::connectToServer(const QString& strIp)
{
}

bool CNetworkFTP::isConnected()
{
	return true;
}

void CNetworkFTP::sendList(const QStringList& list)
{
    QSettings settings;
    QString url_folder = settings.value("folderFTP").toString();
    QString url_loginFTP = settings.value("loginFTP").toString();
    QString url_passwordFTP = settings.value("passwordFTP").toString();
    QString ip_server = settings.value("ipServer").toString();

    QString url_adress = QString("ftp://%1/%2").arg( ip_server ).arg( url_folder );

    QUrl url( url_adress);
    url.setUserName( url_loginFTP);
    url.setPassword( url_passwordFTP);

    // Create list of uploader    
    foreach ( QString fileName, list )
    {
       CNetworkFTPUploader * ptr = new CNetworkFTPUploader ( url, fileName ) ;
        connect ( ptr, SIGNAL( uploadDone() ), this, SLOT ( onUploadDone() ) );
        m_uploaderList.push_back( ptr );
    }

    if ( m_uploaderList.size() > 0 )
    {
        m_uploaderList[0]->send();
    }
}

void CNetworkFTP::onUploadDone()
{
    if ( !m_uploaderList[ m_iter ]->isFinished() )
    {
        emit errorOccur( CError::NETWORKPROBLEM );
        return;
    }
    else
    {
        emit fileSent();
        // Send next file
        ++m_iter;
        if ( m_iter < m_uploaderList.size() )
        {
            m_uploaderList[ m_iter ]->send();
        }
        else
        {
            emit allFilesSent();
        }
    }
}

// -------------------------------------------------------------------------------


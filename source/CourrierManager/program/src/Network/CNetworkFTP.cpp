#include "Network/CNetworkFTP.h"

// #include <QDataStream>
// #include <QFile>
// #include <QBuffer>

#include "CError.h"

#include <QFile>
#include <QFileInfo>
#include <QSettings>

#include <QTimer>
#include <memory>

#define TIMEOUT 10000

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
    m_file.reset( new QFile ( m_fileName ) );
    if (m_file->open(QIODevice::ReadOnly))
    {
        QNetworkRequest request(m_url);
        m_reply = m_access.put(request, m_file.get() );
        m_reply->ignoreSslErrors();
        connect(m_reply, SIGNAL(finished()), this,  SIGNAL(uploadDone() ) );
        connect(m_reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &CNetworkFTPUploader::uploadError);
    } 
}

bool CNetworkFTPUploader::isFinished()
{
    if ( !m_reply )
    {
        return true;
    }

    return ( m_reply && 
             m_reply->isFinished() && 
             m_reply->error() == QNetworkReply::NoError ) ;
}

QNetworkReply::NetworkError CNetworkFTPUploader::getError() const
{
    if ( m_reply )
    {
        return m_reply->error();   
    }
    else
    {
        return QNetworkReply::NoError;
    }
}

void CNetworkFTPUploader::deleteFile()
{
    if ( m_file->isOpen() )
    {
        m_file->close();
    }

    QFile::remove( m_fileName);
}

// --------------------------------------------------------------------------------------------------

CNetworkFTP::CNetworkFTP()
    : CNetwork()
    , m_iter(0)
    , m_error_occured( false )
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
    QString ftp_adress = QString("ftp://%1/%2").arg(ip_server).arg(url_folder);

    // Create list of uploader    
    foreach(QString full_filename_path, list)
    {
        QFileInfo info(full_filename_path);
        QString current_adress = ftp_adress;
        current_adress.append(info.fileName());

        QUrl url(current_adress);
        url.setUserName(url_loginFTP);
        url.setPassword(url_passwordFTP);
        qDebug() << "Current adress: " << current_adress;
        CNetworkFTPUploader * ptr = new CNetworkFTPUploader(url, full_filename_path);
        connect(ptr, SIGNAL(uploadDone()), this, SLOT(onUploadDone()));
        connect(ptr, &CNetworkFTPUploader::uploadError, this, &CNetworkFTP::onNetworkError);
        m_uploaderList.push_back(ptr);

    }

    if (m_uploaderList.size() > 0)
    {
        m_uploaderList[0]->send();
    }
}

void CNetworkFTP::onUploadDone()
{
    if ( !m_uploaderList[ m_iter ]->isFinished() )
    {
        // ContentAccessDenied si le fichier existe déjà
        if (m_uploaderList[ m_iter ]->getError() == QNetworkReply::ContentAccessDenied )
        {
            if ( !m_error_occured )
            {
                emit errorOccur ( CError::NETWORKFILEEXISTS);
                m_error_occured = true;
            }
        }
        else     
        {
            emit errorOccur( CError::NETWORKPROBLEM );
            return;
        }
    }

    emit fileSent();
    // Delete current file
    m_uploaderList[ m_iter ]->deleteFile();

    // Send next file
    ++m_iter;
    if (m_iter < m_uploaderList.size())
    {
        m_uploaderList[m_iter]->send();
    }
    else
    {
        emit allFilesSent();
    }

}

void CNetworkFTP::onNetworkError(QNetworkReply::NetworkError code)
{
         qDebug() << "Erreur: " << code;
   }

// -------------------------------------------------------------------------------


#include "Network/CNetworkFTP.h"

// #include <QDataStream>
// #include <QFile>
// #include <QBuffer>

#include "CError.h"

#include <QNetworkAccessManager>
#include <QUrl>

CNetworkFTP::CNetworkFTP():
	CNetwork()/*,
    m_iter(0)*/
{

    m_access.reset(new QNetworkAccessManager());

    QSettings settings;
    QString url_adress;
    QString ip_server = settings.value("ipServer").toString();
    QString url_folder = settings.value("folderFTP").toString();
    QString url_loginFTP = settings.value("loginFTP").toString();
    QString url_passwordFTP = settings.value("passwordFTP").toString();
   
    QString url_adress = QString("ftp://%1/%2");
    QUrl url ( url_adress );
    url.setUserName( url_loginFTP );
    url.setPassword( url_passwordFTP );
 
//    QFile data( filename )
//    if (data->open(QIODevice::ReadOnly)) {
//         reply = m_access.put(QNetworkRequest(url), data);
//         connect(reply, SIGNAL(uploadProgress(qint64, qint64)), SLOT(uploadProgress(qint64, qint64)));
//         connect(reply, SIGNAL(finished()), SLOT(uploadDone()));

//        http://www.qtcentre.org/threads/51348-Putting-large-size-file-on-FTP-using-QNetworkAccessManager
//     }
}

CNetworkFTP::~CNetworkFTP()
{
//    m_socket.close();
}

void CNetworkFTP::connectToServer(const QString& strIp)
{
    //m_socket.connectToHost(strIp, 4500);
    //connect(&m_socket, SIGNAL(connected()), this, SIGNAL(connectToHost()));
    //connect(&m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnectedFromHost()));
    //connect(&m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

bool CNetworkFTP::isConnected()
{
//    return m_socket.isOpen();
	return false;
}

void CNetworkFTP::sendList(const QStringList& list)
{
    //m_list = list;
    //m_iter = 0;
    //if (isConnected()) {
    //    if (list.count() > 0) {
    //        sendFile(list.at(0));
    //    }
    //}
}


void CNetworkFTP::sendFile(const QString& strFilePath)
{
    //QFile fileToSend(strFilePath);
    //fileToSend.open(QIODevice::ReadOnly);
    //QByteArray ba ;       // Construct a QByteArray object
    //QDataStream data(&ba, QIODevice::WriteOnly);
    //data.setVersion(QDataStream::Qt_4_0);
    //data << (quint64)0; // Reserve space for a 16 bit integer that will contain the total size of the data block we are sending.
    //data << strFilePath;
    //data << fileToSend.readAll();
    //data.device()->seek(0);
    //data << (quint64)(ba.size() - sizeof(quint64));
    //m_socket.write(ba);
}


QString  CNetworkFTP::getNextFile()
{
    //++m_iter;
    //if (m_iter < m_list.count()) {
    //    return m_list[m_iter];
    //} else {
    //    return QString();
    //}

	return QString();
}


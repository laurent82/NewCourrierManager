#include "Network/CNetworkServer.h"

#include <QDataStream>
#include <QFile>
#include <QBuffer>
#include "CError.h"

CNetworkServer::CNetworkServer() :
    m_iter(0)
{

}

CNetworkServer::~CNetworkServer()
{
    m_socket.close();
}

void CNetworkServer::connectToServer(const QString& strIp)
{
    m_socket.connectToHost(strIp, 4500);
    connect(&m_socket, SIGNAL(connected()), this, SIGNAL(connectToHost()));
    connect(&m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnectedFromHost()));
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

bool CNetworkServer::isConnected()
{
    return m_socket.isOpen();
}

void CNetworkServer::sendList(const QStringList& list)
{
    m_list = list;
    m_iter = 0;
    if (isConnected()) {
        if (list.count() > 0) {
            sendFile(list.at(0));
        }
    }
}


void CNetworkServer::sendFile(const QString& strFilePath)
{
    QFile fileToSend(strFilePath);
    fileToSend.open(QIODevice::ReadOnly);
    QByteArray ba ;       // Construct a QByteArray object
    QDataStream data(&ba, QIODevice::WriteOnly);
    data.setVersion(QDataStream::Qt_4_0);
    data << (quint64)0; // Reserve space for a 16 bit integer that will contain the total size of the data block we are sending.
    data << strFilePath;
    data << fileToSend.readAll();
    data.device()->seek(0);
    data << (quint64)(ba.size() - sizeof(quint64));
    m_socket.write(ba);
}

void CNetworkServer::onReadyRead()
{
  /*  
	// FIXME
	int size =  m_socket.bytesAvailable();
    QByteArray ba = m_socket.read(size);
    QString msg = ba.data();
    if (msg.contains("ACK")) {
        // Suppression du fichier courant.
        QFile::remove(m_list[m_iter]);
        QString nextFile = getNextFile();
        if (nextFile.isEmpty()) {
            emit allFilesSent();
        } else {
            emit fileSent();
            sendFile(nextFile);
        }
    } else if (msg.contains("ERR")) {
        emit errorOccur(CError::NETWORKPROBLEM);
    }
	*/
}

QString  CNetworkServer::getNextFile()
{
    ++m_iter;
    if (m_iter < m_list.count()) {
        return m_list[m_iter];
    } else {
        return QString();
    }
}


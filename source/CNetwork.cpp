#include "CNetwork.h"

#include <QDataStream>
#include <QFile>
#include <QBuffer>

CNetwork::CNetwork()
{

}

void CNetwork::connectToServer(const QString& strIp)
{
    qDebug() << "Essai de se connecter à l'adresse : " << strIp;
    m_socket.connectToHost(strIp, 3210);
    connect(&m_socket, SIGNAL(connected()), this, SIGNAL(connectToHost()));
    connect(&m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnectedFromHost()));
}

bool CNetwork::isConnected()
{
    return m_socket.isOpen();
}

void sendList(const QStringList list)
{
    if (isConnected()) {
        foreach (QString strFileName, list) {
            sendFile
        }
    }
}


void CNetwork::sendFile(const QString& strFilePath)
{
    QFile fileToSend(strFilePath);
    fileToSend.open(QIODevice::ReadOnly);
    QByteArray ba ;       // Construct a QByteArray object
    QBuffer buffer(&ba); // Construct a QBuffer object using the QByteArray.

//    QDataStream data(m_socket);
//    QFile file(strFilePath);
//    if (!file.open(QIODevice::ReadOnly))
//      return;
//    QByteArray ba = file.readAll();
//    qint64 size = ba.size();
//    data << size;
//    data << ba;
//    file.close();
}

#include "CNetwork.h"

#include <QDataStream>
#include <QFile>
#include <QBuffer>

CNetwork::CNetwork()
{

}

void CNetwork::connectToServer(const QString& strIp)
{
    qDebug() << "Essai de se connecter � l'adresse : " << strIp;
    m_socket.connectToHost(strIp, 3210);
    connect(&m_socket, SIGNAL(connected()), this, SIGNAL(connectToHost()));
    connect(&m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnectedFromHost()));
}

bool CNetwork::isConnected()
{
    return m_socket.isOpen();
}

void CNetwork::sendList(const QStringList& list)
{
    if (isConnected()) {
        foreach (QString strFileName, list) {
            sendFile(strFileName);
        }
    }
}


void CNetwork::sendFile(const QString& strFilePath)
{
    QFile fileToSend(strFilePath);
    fileToSend.open(QIODevice::ReadOnly);
    QByteArray ba ;       // Construct a QByteArray object
    QDataStream data(&ba, QIODevice::WriteOnly);
    data.setVersion(QDataStream::Qt_4_0);
    data << (quint16)0; // Reserve space for a 16 bit integer that will contain the total size of the data block we are sending.
    data << fileToSend.readAll();
    m_socket.write(ba);
//    QFile file(strFilePath);
//    if (!file.open(QIODevice::ReadOnly))
//      return;
//    QByteArray ba = file.readAll();
//    qint64 size = ba.size();
//    data << size;
//    data << ba;
//    file.close();
}

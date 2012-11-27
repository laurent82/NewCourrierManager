#include "CNetwork.h"

#include <QDataStream>
#include <QFile>

CNetwork::CNetwork()
{

}

void CNetwork::connectToServer(const QString& strIp)
{
    m_socket.connectToHost(strIp, 3210);
    connect(&m_socket, SIGNAL(connected()), this, SIGNAL(connectToHost()));
    connect(&m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnectedFromHost()));
}

void CNetwork::sendFile(const QString& strFilePath)
{
    QDataStream data(m_socket);
    QFile file(strFilePath);
    if (!file.open(QIODevice::ReadOnly))
      return;
    QByteArray ba = file.readAll();
    qint64 size = ba.size();
    data << size;
    data << ba;
    file.close();
}

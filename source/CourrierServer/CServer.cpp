#include "CServer.h"
#include <QDebug>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QDataStream>
#include <QTcpSocket>
#include <QFile>
#include <QStringList>

CServer::CServer(QObject *parent) :
    QObject(parent),
    m_transferDir("transfert/"),
    m_backupDir("backupDir/")
{
    m_server = new QTcpServer(this);


    connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty()) {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }

    if (!m_server->listen(QHostAddress(ipAddress), 4500)) {
        qDebug() << "Impossible de dmarrer le serveur.";
        exit(-1);
    }
    qDebug() << "Server is running on : " <<  m_server->serverAddress() << m_server->serverPort();

}

void CServer::onNewConnection()
{
    m_socket = m_server->nextPendingConnection();
    if (m_socket) {
        connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
        qDebug() << "New client connected.";
    } else {
        qDebug() << "No pending connection";
    }
}

void CServer::onDisconnected()
{
     disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
     disconnect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
     qDebug() << "Client disconnected";
     m_socket->deleteLater();
     m_socket = 0;
}

void CServer::onReadyRead()
{
    if (m_socket) {
        QByteArray ba;
        QString fileName;
        qint64 size;
        QDataStream data (m_socket);
        data >> size;
        do {
            m_socket->waitForReadyRead(1000);
        } while (m_socket && m_socket->bytesAvailable() < size);
        data >> fileName;
        data >> ba;
        // Copy file
        fileName = (fileName.split("/")).last();
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(ba);
            file.close();
            sendACK();
        } else {
            qDebug() << "Could not create file " << fileName;
            sendERR();
        }
        if (!m_socket) {
            qDebug() << "Disconnected during the file transfert";
            sendERR();
        }
    } else {
        sendERR();
        qDebug() << "Error socket";
    }
}

void CServer::sendACK(){
    if (!m_socket){
        return;
    }
    QByteArray ba;
    ba.append("ACK");
    m_socket->write(ba);
}

void CServer::sendERR(){
    if (!m_socket){
        return;
    }
    QByteArray ba;
    ba.append("ERR");
    m_socket->write(ba);
}

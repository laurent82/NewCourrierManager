#ifndef CNETWORKSERVER_H
#define CNETWORKSERVER_H

#include "Network/CNetwork.h"

#include <QString>
#include <QStringList>
#include <QtNetwork/QTcpSocket>


class CNetworkServer : public CNetwork
{
    Q_OBJECT
public:
    CNetworkServer();
    ~CNetworkServer();
    virtual void connectToServer(const QString& strIp) override;
    virtual bool isConnected() override;
    virtual void sendList(const QStringList &list) override;
    void sendFile(const QString& strFilePath);

private slots:
    void onReadyRead();

private:
    QString getNextFile();
    QTcpSocket m_socket;
    QStringList m_list;
    int m_iter;

};

#endif // CNETWORKSERVER_H

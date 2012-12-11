#ifndef CNETWORK_H
#define CNETWORK_H

#include <QString>
#include <QStringList>
#include <QtNetwork/QTcpSocket>


class CNetwork : public QObject
{
    Q_OBJECT
public:
    CNetwork();
    void connectToServer(const QString& strIp = "127.0.0.1");
    bool isConnected();
    void sendList(const QStringList &list);
    void sendFile(const QString& strFilePath);

signals:
    void connectToHost();
    void disconnectedFromHost();

private:
    QTcpSocket m_socket;

};

#endif // CNETWORK_H

#ifndef CSERVER_H
#define CSERVER_H

#include <QObject>
#include <QTcpServer>

class CServer : public QObject
{
    Q_OBJECT
public:
    explicit CServer(QObject *parent = 0);
    
private:
    void sendACK();
    void sendERR();
signals:
    
public slots:
    void onNewConnection();
    void onDisconnected();
    void onReadyRead();

private:
    QTcpServer* m_server;
    QTcpSocket* m_socket;
    QString m_transferDir;
    QString m_backupDir;
    
};

#endif // CSERVER_H

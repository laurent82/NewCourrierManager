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
    ~CNetwork();
    void connectToServer(const QString& strIp);
    bool isConnected();
    void sendList(const QStringList &list);
    void sendFile(const QString& strFilePath);

private slots:
    void onReadyRead();

signals:
    void fileSent();
    void allFilesSent();
    void connectToHost();
    void disconnectedFromHost();
    void errorOccur(int);

private:
    QString getNextFile();
    QTcpSocket m_socket;
    QStringList m_list;
    int m_iter;

};

#endif // CNETWORK_H

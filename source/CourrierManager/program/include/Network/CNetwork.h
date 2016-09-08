#ifndef CNETWORK_H
#define CNETWORK_H

#include <QString>
#include <QStringList>
#include <QtNetwork/QTcpSocket>


class CNetwork : public QObject
{
    Q_OBJECT
public:
	CNetwork() = default;
	~CNetwork() = default;
	virtual void connectToServer(const QString& strIp) = 0;
	virtual bool isConnected() = 0;
    virtual void sendList(const QStringList &list) = 0;
    virtual void sendFile(const QString& strFilePath) = 0;

protected:
	virtual QString getNextFile() = 0;
//private slots:
//    void onReadyRead();

//signals:
//    void fileSent();
//    void allFilesSent();
//    void connectToHost();
//    void disconnectedFromHost();
//    void errorOccur(int);
//
// private:
   
    //QTcpSocket m_socket;
    //QStringList m_list;
    //int m_iter;

};

#endif // CNETWORK_H

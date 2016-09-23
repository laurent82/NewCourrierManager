#ifndef CNETWORK_H
#define CNETWORK_H

#include <QString>
#include <QStringList>
#include <QObject>

class CNetwork : public QObject
{
    Q_OBJECT
public:
	CNetwork() = default;
	~CNetwork() = default;
	virtual void connectToServer(const QString& strIp) = 0;
	virtual bool isConnected() = 0;
    virtual void sendList(const QStringList &list) = 0;

signals:
    void fileSent();
    void allFilesSent();
    void connectToHost();
    void disconnectedFromHost();
    void errorOccur(int);

};

#endif // CNETWORK_H

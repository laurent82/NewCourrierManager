#ifndef CNETWORKFTP_H
#define CNETWORKFTP_H

#include "CNetwork.h"
#include <QString>
#include <QStringList>
#include <QtNetwork/QTcpSocket>


class CNetworkFTP : public CNetwork
{
    Q_OBJECT
public:
	CNetworkFTP();
    ~CNetworkFTP();
    virtual void connectToServer(const QString& strIp);
    virtual bool isConnected();
    virtual void sendList(const QStringList &list);
    virtual void sendFile(const QString& strFilePath);

protected:
	virtual QString getNextFile();

};

#endif // CNETWORKFP_H

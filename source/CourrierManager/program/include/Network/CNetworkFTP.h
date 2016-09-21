#ifndef CNETWORKFTP_H
#define CNETWORKFTP_H

#include "CNetwork.h"

#include <QString>
#include <QStringList>

#include <memory>

class QNetworkAccessManager;

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

private:
    std::unique_ptr<QNetworkAccessManager> m_access;

};

#endif // CNETWORKFP_H

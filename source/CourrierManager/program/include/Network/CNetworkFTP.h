#ifndef CNETWORKFTP_H
#define CNETWORKFTP_H

#include "CNetwork.h"

#include <QString>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QThread>

#include <memory>

class QNetworkReply;

class CNetworkFTPUploader : public QObject
{
    Q_OBJECT
public:
    CNetworkFTPUploader( QUrl& url, const QString& fileName );
    ~CNetworkFTPUploader();

    void send();

    bool isFinished();

signals:
    void uploadDone();

private:
     QNetworkAccessManager m_access;
     QUrl m_url;
     QString m_fileName;
     QNetworkReply * m_reply;

};

class CNetworkFTP : public CNetwork
{
    Q_OBJECT
public:
	CNetworkFTP();
    ~CNetworkFTP();
    virtual void connectToServer(const QString& strIp) override;
    virtual bool isConnected() override;
    virtual void sendList(const QStringList &list) override;

private slots:
    void onUploadDone();

private:
    QList<CNetworkFTPUploader*> m_uploaderList;
    int m_iter;
};

#endif // CNETWORKFP_H

#ifndef CNETWORKFTP_H
#define CNETWORKFTP_H

#include "CNetwork.h"

#include <QString>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QThread>

#include <memory>


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
    void uploadError(QNetworkReply::NetworkError code);
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
    void onNetworkError( QNetworkReply::NetworkError code );

private:
    QList<CNetworkFTPUploader*> m_uploaderList;
    int m_iter;
};

#endif // CNETWORKFP_H

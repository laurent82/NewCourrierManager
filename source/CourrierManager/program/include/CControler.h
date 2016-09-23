#ifndef CCONTROLER_H
#define CCONTROLER_H

class CView;
class CFileManager;
class CNetwork;
class CAbstractFilter;

#include <QObject>
#include <QList>
#include <QSettings>

#include <memory>

class CControler : public QObject
{
    Q_OBJECT
public:
    CControler();
    ~CControler();


public slots:
    void onError(int errorId);
    void onCriticalError();
    void onCommandReceived(QString);
    void showConfigDialog();
    void onFileSent();
    void onAllFilesSent();
    void onInfoReceived(QString key, QVariant value);

signals:
    void errorOccur(int);

private:
	void initNetwork();

    QString m_ip;
    std::unique_ptr<CView> m_view;
    std::unique_ptr<CFileManager> m_fileManager;
    std::unique_ptr<CNetwork> m_network;
    QList<CAbstractFilter*> m_filterList;

};
    #endif

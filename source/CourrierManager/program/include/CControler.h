#ifndef CCONTROLER_H
#define CCONTROLER_H

class CView;
class CFileManager;
class CNetwork;
class CAbstractFilter;

#include <QObject>
#include <QList>
#include <QSettings>

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
    CView* m_view;
    CFileManager* m_fileManager;
    CNetwork* m_network;
    QList<CAbstractFilter*> m_filterList;

};
    #endif

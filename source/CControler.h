#ifndef CCONTROLER_H
#define CCONTROLER_H

class CView;
class CFileManager;
class CNetwork;

#include <QObject>

class CControler : public QObject
{
    Q_OBJECT
public:
    CControler();
    ~CControler();


public slots:
    void onError(int errorId);
    void onCriticalError();

    void showConfigDialog();

private:
    CView* m_view;
    CFileManager* m_fileManager;
    CNetwork* m_network;
};
    #endif

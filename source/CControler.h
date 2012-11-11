#ifndef CCONTROLER_H
#define CCONTROLER_H

class CView;
class CFileManager;

#include <QObject>

class CControler : public QObject
{
    Q_OBJECT
public:
    CControler();


public slots:
    void onError(int errorId);
    void onCriticalError();

    void showConfigDialog();

private:
    CView* m_view;
    CFileManager* m_fileManager;
};
    #endif

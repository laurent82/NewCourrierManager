#ifndef CCONTROLER_H
#define CCONTROLER_H

class CView;
class CGestion;

#include <QObject>

class CControler : public QObject
{
    Q_OBJECT
public:
    CControler();

public slots:
    void onCriticalError();

private:
    CView* m_view;
    CGestion* m_gestion;
};
    #endif

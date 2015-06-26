#ifndef COCRINTERFACE
#define COCRINTERFACE

#include <QImage>
#include <QThread>
#include <QString>

class COcrEngine : public QThread
{
public:
    void setInput(const QImage& image);
    QString getOutput();
    virtual void run() = 0;

protected:
    QImage m_image;

};

#endif

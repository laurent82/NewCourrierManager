#ifndef COCRINTERFACE
#define COCRINTERFACE

#include <QImage>
#include <QThread>
#include <QString>
#include <CAbstractPlugin.h>

class COcrEngine : public QThread, public CAbstractPlugin
{
public:
    COcrEngine();
    ~COcrEngine();

    virtual void run();

    void setInput( const QImage& image );
    QString getOutput() const;

private:
    QImage m_image;
    QString m_text;

};

#endif

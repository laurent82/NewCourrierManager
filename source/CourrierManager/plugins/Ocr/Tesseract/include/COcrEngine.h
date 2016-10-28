#ifndef COCRINTERFACE
#define COCRINTERFACE

#include <QImage>
#include <QThread>
#include <QString>
#include <memory>
#include <CAbstractPlugin.h>

namespace tesseract
{
    class TessBaseAPI;
}

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
    std::unique_ptr<tesseract::TessBaseAPI>  m_api;

};

#endif

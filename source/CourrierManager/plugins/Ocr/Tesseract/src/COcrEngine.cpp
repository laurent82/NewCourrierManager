#include "COcrEngine.h"

#include <memory>
#include <baseapi.h>

namespace{

struct RawImage
{
    uchar * data;
    int width;
    int height;
    int bpp; // byte per pixel
    int bpl; // byte per line
};

// -------------------------------------------------------------

void convertToRaw(QImage& image, RawImage& raw)
{
    raw.data = image.bits();
    raw.width = image.width();
    raw.height = image.height();
    raw.bpp = image.depth() / 8;
    raw.bpl = image.bytesPerLine();
}

}
// -------------------------------------------------------------


COcrEngine::COcrEngine()
    :CAbstractPlugin()
{
    setName( "ocr" );
}

COcrEngine::~COcrEngine()
{

}

void COcrEngine::setInput( const QImage& image )
{
    m_image = image;
    start();
}

QString COcrEngine::getOutput() const
{
    return m_text;
}

void COcrEngine::run()
{
    if ( m_image.isNull() )
    {
        m_text = QString();
        return;
    }

    std::unique_ptr<tesseract::TessBaseAPI> api( new tesseract::TessBaseAPI() );
    if (api->Init(NULL, "eng")) {
        m_text = QString();
        return;
    }

    ::RawImage raw;
    ::convertToRaw( m_image, raw);
    
    api->SetImage(raw.data, raw.width, raw.height, raw.bpp, raw.bpl);
    m_text = QString( api->GetUTF8Text() );
    api->End();
}






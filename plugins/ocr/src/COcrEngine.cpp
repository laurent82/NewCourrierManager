#include "COcrEngine.h"


void COcrEngine::setInput(const QImage& image)
{
    m_image = image;
}


QString COcrEngine::getOutput()
{
    return QString();
}

#include "Ocr/COcrManager.h"

OcrManager& OcrManager::instance()
{
    static OcrManager instance;
    return instance;
}

OcrManager::OcrManager():
    m_engine( nullptr )
{

}

bool OcrManager::isActive() const
{
    return m_engine != nullptr;
}

void OcrManager::setEngine(OcrEngine * engine)
{
    m_engine = engine;
}

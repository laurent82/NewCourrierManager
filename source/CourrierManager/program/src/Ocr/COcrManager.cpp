#include "Ocr/COcrManager.h"
#include "Ocr/COcrAnalyzer.h"
#include <plugins_config.h>

#ifdef CM_WITH_OCR
    #include <COcrEngine.h>
#endif

#include <QDebug>

COcrManager& COcrManager::instance()
{
    static COcrManager instance;
    return instance;
}

COcrManager::COcrManager():
    m_engine( nullptr )
{
    m_analyzer.reset( new COcrAnalyzer() );
    connect( m_analyzer.get(), &COcrAnalyzer::sendInfo, this, &COcrManager::sendInfo );
}

bool COcrManager::isActive() const
{
    return m_engine != nullptr;
}

void COcrManager::setEngine( COcrEngine* engine)
{
#ifdef CM_WITH_OCR
    if ( engine )
    {
        m_engine = engine;
        connect( m_engine, &COcrEngine::finished, this, &COcrManager::analyzeText );
    }
#endif
}

void COcrManager::setInput(QImage image)
{
    if ( isActive() )
    {
#ifdef CM_WITH_OCR
        m_engine->setInput( image );
#endif
    }
}

void COcrManager::setPatientList(QStringList* list)
{
    m_analyzer->setPatientList( list );
}

void COcrManager::analyzeText()
{
#ifdef CM_WITH_OCR
    if ( m_analyzer->isRunning() )
    {
        m_analyzer->stop();
        m_analyzer->wait();
    }
    m_analyzer->setInput( m_engine->getOutput() );

#endif
}


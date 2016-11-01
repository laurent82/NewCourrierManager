#include "Ocr/COcrManager.h"
#include "Ocr/COcrAnalyzer.h"
#include <plugins_config.h>
#include <QSettings>
#if CM_WITH_OCR
    #include <COcrEngine.h>
#endif

#include <QDebug>

COcrManager::COcrManager():
    m_engine( nullptr ),
    m_active( true ),
    m_imageHasBeenSkipped( false )
{
    m_analyzer.reset( new COcrAnalyzer() );
    connect( m_analyzer.get(), &COcrAnalyzer::sendInfo, this, &COcrManager::sendInfo );

    QSettings settings;
    m_active = settings.value("ocr_enabled").toBool();
}

bool COcrManager::isActive() const
{
    return ( m_active && m_engine != nullptr);
}

void COcrManager::setEngine( CAbstractPlugin* engine)
{
#if CM_WITH_OCR
    if ( engine )
    {
        m_engine = dynamic_cast<COcrEngine*>( engine );
        if ( m_engine != nullptr )
        {
            connect( m_engine, &COcrEngine::finished, this, &COcrManager::analyzeText );
        }
    }
#endif
}

void COcrManager::setInput(QImage image)
{
    if ( isActive() )
    {
#if CM_WITH_OCR
        if ( !m_engine->isRunning())
        {
            if ( m_analyzer->isRunning() )
            {
                m_analyzer->stop();
            }
            m_imageHasBeenSkipped = false;
            m_engine->setInput( image );
        }
        else
        {
            m_imageHasBeenSkipped = true;
        }
#endif
    }
}

void COcrManager::setPatientList(QStringList* list)
{
    m_analyzer->setPatientList( list );
}

void COcrManager::analyzeText()
{
#if CM_WITH_OCR
    if ( !m_imageHasBeenSkipped )
    {
        m_analyzer->setInput( m_engine->getOutput() );
    }
#endif
}


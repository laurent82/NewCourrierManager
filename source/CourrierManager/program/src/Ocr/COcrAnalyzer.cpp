#include "Ocr/COcrAnalyzer.h"

#include <QDebug>

COcrAnalyzer::COcrAnalyzer()
{

}

void COcrAnalyzer::setPatientList(QStringList* list)
    : QThread()
{
    m_patientList = list;
}

void COcrAnalyzer::setInput(const QString & text)
{
    m_ocrText = text;
    start();
}

void COcrAnalyzer::run()
{
    QStringList splitted = m_ocrText.split(QRegExp("\\s+"), QString::SkipEmptyParts );
    QStringList response;
    foreach( QString word, splitted )
    {
        if ( !m_allowedToRun )
        {
            return;
        }

        QString analyze( word );
        
    }

    emit sendInfo ( "ocr_list", response );
}

void COcrAnalyzer::stop()
{
    m_allowedToRun = false;
}

QString COcrAnalyzer::analyze(QString word)
{
    float bestResult;
    QString bestPatientName;
    foreach( QString patientFullName, m_patientList )
    {
        QStringList list = patientFullName.split(";");
        foreach( QString subName, list )
        {
            float current_score = scoredAnalyze( word, subName );
        }
    }
}

float COcrAnalyzer::scoredAnalyze(QString word, QString target)
{
    if ( word.compare( target, Qt::CaseInsensitive ) == 0  )
    {
        return 1.0;
    }

}

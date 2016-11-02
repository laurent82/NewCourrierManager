#include "Ocr/COcrAnalyzer.h"
#include "CDate.h"

#include <QDebug>
#include <QSettings>

COcrAnalyzer::COcrAnalyzer() 
    : QThread()
    , m_allowedToRun( false )
    , m_patientList( nullptr )
    , m_threshold( 1.2f )
    , m_malus( 0.2f )
{
    QSettings settings;
    m_threshold = settings.value("ocr_threshold").toFloat();
    m_malus = settings.value("ocr_malus").toFloat();
}

void COcrAnalyzer::setPatientList(QStringList* list)

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
    if ( m_patientList == nullptr )
    {
        qDebug() << "Pas de liste, analyse impossible";
        return;
    }

    QStringList splitted = m_ocrText.split(QRegExp("\\s+"), QString::SkipEmptyParts );
    splitted.removeDuplicates();

    QStringList response;
    m_allowedToRun = true;


    foreach( QString word, splitted )
    {
        if ( !m_allowedToRun )
        {
            return;
        }

        word.remove(QRegExp("[.?!;,:]"));

        if ( CDate::isDate( word ) )
        {
            emit sendInfo ( "ocr_date", word );    
        }
        else
        {
            QString current_analyze = analyze(word);
            if (!current_analyze.isEmpty() && !response.contains(current_analyze))
            {
                response.append(current_analyze);
            }
        }
        
    }

    emit sendInfo ( "ocr_list", response );
}

void COcrAnalyzer::stop()
{
    m_allowedToRun = false;
}

QString COcrAnalyzer::analyze(QString word)
{

    float bestResult = 0; 
    QString bestPatientName;
    foreach( QString patientFullName, *m_patientList )
    {
        QStringList list = patientFullName.split(", ");
        float totalScore = 0;
        foreach( QString subName, list )
        {
            totalScore += scoredAnalyze( word, subName );
        }

        if  ( totalScore > bestResult )
        {
            bestResult = totalScore;
            bestPatientName = patientFullName;
        }
    }
   
    if (  bestResult > m_threshold )
    {
        return bestPatientName;
    }
    else
    {
        return QString();
    }
}

float COcrAnalyzer::scoredAnalyze(QString word, QString target)
{
    if ( word.compare( target, Qt::CaseInsensitive ) == 0  )
    {
        return 1.0;
    }
    else
    {
        return deepWordCompare( word, target );
    }

}

float COcrAnalyzer::deepWordCompare(QString word, QString target)
{
    // Taille trop differente
    if ( (word.size() < target.size() -1) 
         || ( word.size() >  target.size() + 1) )
    {
        return 0.0;
    }

    // Ne compare que si les deux mots ont la meme premiere lettre.
    if ( word[0].toLower() != target[0].toLower() )
    {
        return 0.0; 
    }

    float current_score = 0;

    // Taille egale
    if ( word.size() == target.size() )
    {
        for (int i = 0; i < word.size(); ++i )
        {
            if ( word[i].toLower() == target[i].toLower() )
            {
                ++current_score;
            }
        }

        return current_score/(float)word.size();
    }

    // Taille proche
    float bigger_size = word.size() > target.size() ? word.size() : target.size();
    for ( int i = 0; i < word.size(); ++i )
    {
        int pos = ( i < 1 ) ? 1 : i;
        pos = ( pos < target.size() - 2 ) ? target.size() - 2 : pos;

       if ( target.mid( pos, 3 ).indexOf( word[i] ) != -1 )
       {
            ++current_score;        
       }

    }
    
    return current_score / (float)bigger_size - m_malus;

}

#ifndef OCRANALYZER_H
#define OCRANALYZER_H

#include <QString>
#include <QThread>

class COcrAnalyzer : public QThread
{
    Q_OBJECT
public:
    COcrAnalyzer();
    void setInput( const QString & text );
    void setPatientList( QStringList* list);
    void stop();
    virtual void run();

signals:
    void sendInfo( QString, QVariant );

private:
    QString analyze( QString word );
    float scoredAnalyze( QString word, QString target );
    float deepWordCompare(QString word, QString target);

    QString m_ocrText;
    QStringList* m_patientList;
    bool m_allowedToRun;

    float m_threshold;
    float m_malus;

};

#endif 
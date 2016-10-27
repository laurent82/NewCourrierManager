#ifndef OCRMANAGER_H
#define OCRMANAGER_H

#include <QObject>
#include <QImage>
#include <QString>
#include <QVariant>
#include <memory>

class COcrEngine;
class COcrAnalyzer;

class COcrManager : public QObject 
{
    Q_OBJECT
public:
    static COcrManager& instance();   

    void setEngine( COcrEngine* engine );
    void setPatientList(QStringList* list);

    bool isActive() const;

    void setInput( QImage image );

    COcrManager(COcrManager const&) = delete;
    void operator=(COcrManager const&) = delete;

protected:
     COcrManager();

signals:
    void sendInfo(QString key, QVariant value);

private slots:
    void analyzeText();

private:
    std::unique_ptr<COcrAnalyzer> m_analyzer; 
    COcrEngine* m_engine;
};

#endif 
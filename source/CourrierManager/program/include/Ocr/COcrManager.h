#ifndef OCRMANAGER_H
#define OCRMANAGER_H

#include <QObject>
#include <QImage>
#include <QString>
#include <QVariant>
#include <memory>

class COcrEngine;
class COcrAnalyzer;
class CAbstractPlugin;

class COcrManager : public QObject 
{
    Q_OBJECT
public:
    COcrManager();

    void setEngine( CAbstractPlugin* engine);
    void setPatientList(QStringList* list);

    bool isActive() const;

    void setInput( QImage image );

signals:
    void sendInfo(QString key, QVariant value);

private slots:
    void analyzeText();

private:
    std::unique_ptr<COcrAnalyzer> m_analyzer; 
    COcrEngine* m_engine;
    bool m_active;
    bool m_imageHasBeenSkipped;
};

#endif 
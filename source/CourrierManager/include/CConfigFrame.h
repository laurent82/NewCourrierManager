#ifndef CCONFIGFRAME_H
#define CCONFIGFRAME_H

#define USE_QT5 1

#ifdef USE_QT5
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#else
#include <QWidget>
#include <QLineEdit>
#endif

class CConfigFrame : public QFrame
{
    Q_OBJECT
public:
    static CConfigFrame* instance();
signals:
    
protected:
    explicit CConfigFrame(QWidget *parent = 0);

public slots:
    void onValidateClicked();
private:
    void readConfigFile();

    QLineEdit* m_searchLine;
    QLineEdit* m_transferLine;
    QLineEdit* m_destinationLine;
    QLineEdit* m_backupLine;
    QLineEdit* m_PDFLine;
    QLineEdit* m_IPLine;

    static CConfigFrame* m_instance;
};

#endif // CCONFIGFRAME_H

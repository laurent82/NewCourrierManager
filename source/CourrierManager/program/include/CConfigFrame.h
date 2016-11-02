#ifndef CCONFIGFRAME_H
#define CCONFIGFRAME_H

#define USE_QT5 1

#ifdef USE_QT5
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#else
#include <QWidget>
#include <QLineEdit>
#endif

#include <QDialog>

class QRadioButton;

class CConfigFrame : public QDialog
{
    Q_OBJECT
public:
  CConfigFrame(QWidget *parent = 0);

public slots:
    void onValidateClicked();
	void onNetworkMethodChanged();
private:
	QWidget* createDirSettings();
	QWidget* createNetworkSettings();
    QWidget* createOcrSettings();

    void readSettings();

    QLineEdit* m_searchLine;
    QLineEdit* m_transferLine;
    QLineEdit* m_backupLine;
    QLineEdit* m_PDFLine;
    QLineEdit* m_IPLine;
	QRadioButton * m_rbServer;
	QRadioButton * m_rbFTP;
	QLineEdit* m_loginFTP;
	QLineEdit* m_passwordFTP;
	QLineEdit* m_folderFTP;
    QCheckBox* m_ocrEnabled;
    QLineEdit* m_ocrThreshold;
    QLineEdit* m_ocrMalus;

    static CConfigFrame* m_instance;
};

#endif // CCONFIGFRAME_H

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

    void readSettings();

    QLineEdit* m_searchLine;
    QLineEdit* m_transferLine;
    QLineEdit* m_destinationLine;
    QLineEdit* m_backupLine;
    QLineEdit* m_PDFLine;
    QLineEdit* m_IPLine;
	QRadioButton * m_rbServer;
	QRadioButton * m_rbFTP;
	QLineEdit* m_loginFTP;
	QLineEdit* m_passwordFTP;
	QLineEdit* m_folderFTP;

    static CConfigFrame* m_instance;
};

#endif // CCONFIGFRAME_H

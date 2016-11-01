#include "CConfigFrame.h"
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QTabWidget>
#include <QtCore/QDir>
#include <QSettings>
#include <plugins_config.h>

using namespace std;

CConfigFrame::CConfigFrame(QWidget *parent) :
    QDialog(parent)
{
	QVBoxLayout * mainLayout = new QVBoxLayout;
	setLayout(mainLayout);
	setMinimumWidth(500);

	QTabWidget* tabWidget = new QTabWidget;
	tabWidget->addTab(createDirSettings(), QString("Général") );
	tabWidget->addTab(createNetworkSettings(), "Réseau");
#if CM_WITH_OCR
    tabWidget->addTab(createOcrSettings(), "OCR");
#endif


	QPushButton* closeButton = new QPushButton("Valider");
	connect(closeButton, SIGNAL(clicked()), this, SLOT(onValidateClicked()));

	mainLayout->addWidget(tabWidget);
	mainLayout->addWidget(closeButton);


	readSettings();
}

void CConfigFrame::onValidateClicked()
{
    QSettings settings;
    settings.setValue( "searchDir", m_searchLine->text());
    settings.setValue( "transferDir", m_transferLine->text());
    settings.setValue( "destinationDir", m_destinationLine->text());
    settings.setValue( "backupDir", m_backupLine->text());
    settings.setValue( "PDFDir", m_PDFLine->text());
    
	settings.setValue( "ipServer", m_IPLine->text());
	
	if (m_rbFTP->isChecked())
	{
		settings.setValue("network_method", "ftp");
		settings.setValue("loginFTP", m_loginFTP->text());
		settings.setValue("passwordFTP", m_passwordFTP->text());
		settings.setValue("folderFTP", m_folderFTP->text());
	}
	else
	{
		settings.setValue("network_method", "server");
	}

#if CM_WITH_OCR
    settings.setValue("ocr_enabled", m_ocrEnabled->isChecked());
    settings.setValue("ocr_threshold", m_ocrThreshold->text().toFloat());
    settings.setValue("ocr_malus", m_ocrMalus->text().toFloat());
#endif 
	
    close();
}

void CConfigFrame::readSettings()
{
    QSettings settings;

    m_searchLine->setText( settings.value( "searchDir" ).toString() );
    m_transferLine->setText( settings.value( "transferDir" ).toString() );
    m_destinationLine->setText( settings.value( "destinationDir" ).toString() );
    m_backupLine->setText( settings.value( "backupDir" ).toString() );
    m_PDFLine->setText( settings.value( "PDFDir" ).toString() );
    m_IPLine->setText( settings.value( "ipServer" ).toString() );
	if ( settings.value("network_method") == "ftp" )
	{
		m_rbFTP->setChecked(true);
		m_loginFTP->setText(settings.value("loginFTP").toString());
		m_passwordFTP->setText(settings.value("passwordFTP").toString());
		m_folderFTP->setText(settings.value("folderFTP").toString());
	}
	else
	{
		m_rbServer->setChecked(true);
	}
	onNetworkMethodChanged();

#if CM_WITH_OCR
    m_ocrEnabled->setChecked( settings.value("ocr_enabled").toBool() );
    m_ocrThreshold->setText(settings.value("ocr_threshold").toString() );
    m_ocrMalus->setText(settings.value("ocr_malus").toString() );
#endif 
}

QWidget* CConfigFrame::createDirSettings()
{
	QWidget * frame = new QFrame;
	QFormLayout* layout = new QFormLayout;
	frame->setLayout(layout);

	m_searchLine = new QLineEdit;
	m_transferLine = new QLineEdit;
	m_destinationLine = new QLineEdit;
	m_backupLine = new QLineEdit;
	m_PDFLine = new QLineEdit;

	QLabel* currentPath = new QLabel(QDir::currentPath());
	layout->addRow(QString("Current path:"), currentPath);
	layout->addRow(QString("Search"), m_searchLine);
	layout->addRow(QString("Transfer"), m_transferLine);
	layout->addRow(QString("Destination"), m_destinationLine);
	layout->addRow(QString("Backup"), m_backupLine);
	layout->addRow(QString("PDF"), m_PDFLine);

	return frame;
}

QWidget* CConfigFrame::createNetworkSettings()
{
	QWidget * frame = new QFrame;
	QVBoxLayout * mainLayout = new QVBoxLayout;
	frame->setLayout(mainLayout);

	QGridLayout* layout = new QGridLayout;
	
	// Ligne 1 - IP
	m_IPLine = new QLineEdit;
	QLabel * label_serverIP = new QLabel("Serveur IP:");
	layout->addWidget( label_serverIP, 0, 0 );
	layout->addWidget( m_IPLine, 0, 1, 1, 2);

	// Ligne 2 - FTP
	QLabel * label_methode = new QLabel("Méthode");
	m_rbServer = new QRadioButton("CourrierServer");
	m_rbFTP = new QRadioButton("FTP");
	connect(m_rbServer, &QRadioButton::clicked, this, &CConfigFrame::onNetworkMethodChanged);
	connect(m_rbFTP, &QRadioButton::clicked, this, &CConfigFrame::onNetworkMethodChanged);

	layout->addWidget(label_methode, 1, 0);
	layout->addWidget(m_rbServer, 1, 1);
	layout->addWidget(m_rbFTP, 1, 2);

	// Ligne 3 - FTP Login
	QLabel * label_login = new QLabel("Login");
	m_loginFTP = new QLineEdit();
	layout->addWidget(label_login, 2, 0);
	layout->addWidget(m_loginFTP, 2, 1, 1, 2);

	// Ligne 4 - FTP password
	QLabel * label_pass = new QLabel("Password");
	m_passwordFTP = new QLineEdit();
	m_passwordFTP->setEchoMode(QLineEdit::Password);
	layout->addWidget(label_pass, 3, 0);
	layout->addWidget(m_passwordFTP, 3, 1, 1, 2);

	// Ligne 5- FTP folder
	QLabel * label_folder = new QLabel("Répertoire");
	m_folderFTP = new QLineEdit();
	layout->addWidget(label_folder, 4, 0);
	layout->addWidget(m_folderFTP, 4, 1, 1, 2);

	mainLayout->addLayout(layout);
	mainLayout->addStretch();
	return frame;
}

QWidget* CConfigFrame::createOcrSettings()
{
    QWidget * frame = new QFrame;
    QVBoxLayout * mainLayout = new QVBoxLayout;
    frame->setLayout(mainLayout);

    QGridLayout* layout = new QGridLayout;

    // Ligne 1 - Actif
    m_ocrEnabled = new QCheckBox;
    QLabel * label_ocr_enabled = new QLabel("Avec OCR:");
    layout->addWidget(label_ocr_enabled, 0, 0);
    layout->addWidget(m_ocrEnabled, 0, 1 );

    // Ligne 2 - Threshold
    QLabel * ocr_threshold_label = new QLabel("Threshold");
    m_ocrThreshold = new QLineEdit;
    layout->addWidget(ocr_threshold_label, 1, 0);
    layout->addWidget(m_ocrThreshold, 1, 1);

    // Ligne 3 - Malus
    QLabel * malus_label = new QLabel("Malus");
    m_ocrMalus = new QLineEdit();
    layout->addWidget(malus_label, 2, 0 );
    layout->addWidget(m_ocrMalus, 2, 1 );
    
    mainLayout->addLayout(layout);
    mainLayout->addStretch();
    return frame;
}


void  CConfigFrame::onNetworkMethodChanged()
{
	QSettings settings;
	if (m_rbServer->isChecked() )
	{
		m_loginFTP->setEnabled( false );
		m_passwordFTP->setEnabled(false);
		m_folderFTP->setEnabled(false);
	}
	else if (m_rbFTP->isChecked() )
	{
		m_loginFTP->setEnabled(true);
		m_passwordFTP->setEnabled(true);
		m_folderFTP->setEnabled(true);
	}

}


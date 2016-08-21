#include "CConfigFrame.h"
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtCore/QDir>
#include <QSettings>


using namespace std;

CConfigFrame::CConfigFrame(QWidget *parent) :
    QDialog(parent)
{
    QFormLayout* layout = new QFormLayout;
    setLayout(layout);
    setMinimumWidth(500);
    m_searchLine = new QLineEdit;
    m_transferLine = new QLineEdit;
    m_destinationLine = new QLineEdit;
    m_backupLine = new QLineEdit;
    m_PDFLine = new QLineEdit;
    m_IPLine =new QLineEdit;

    QPushButton* closeButton = new QPushButton("Valider");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(onValidateClicked()));

    readSettings();

    QLabel* currentPath = new QLabel(QDir::currentPath());
    layout->addRow(QString("Current path:"), currentPath);
    layout->addRow(QString("Search"), m_searchLine);
    layout->addRow(QString("Transfer"), m_transferLine);
    layout->addRow(QString("Destination"), m_destinationLine);
    layout->addRow(QString("Backup"), m_backupLine);
    layout->addRow(QString("PDF"), m_PDFLine);
    layout->addRow(QString("Server IP:"), m_IPLine);
    layout->addWidget(closeButton);

    move(10, 10);
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

}

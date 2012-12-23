#include "CConfigFrame.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QApplication>

// Lecture, écriture de fichier
// A remplacer avec les fonctions Qt ?
#include <fstream>
#include <string>
using namespace std;

CConfigFrame* CConfigFrame::m_instance = 0;

CConfigFrame* CConfigFrame::instance()
{
    if (!m_instance) {
        m_instance = new CConfigFrame(qApp->activeWindow());
    }
    return m_instance;
}


CConfigFrame::CConfigFrame(QWidget *parent) :
    QFrame(parent)
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

    readConfigFile();

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
    ofstream file("config.txt");
    file << m_searchLine->text().toStdString() << endl;
    file << m_transferLine->text().toStdString() << endl;
    file << m_destinationLine->text().toStdString() << endl;
    file << m_backupLine->text().toStdString() << endl;
    file << m_PDFLine->text().toStdString() << endl;
    file << m_IPLine->text().toStdString() << endl;

    file.close();
    close();
}

void CConfigFrame::readConfigFile()
{
    // Lecture des paramètres
    ifstream file("config.txt");
    if (file) {
        string strSearch, strTransfert, strDestination, strBackup, strPDF, strIP;
        getline(file, strSearch);
        getline(file, strTransfert);
        getline(file, strDestination);
        getline(file, strBackup);
        getline(file, strPDF);
        getline(file, strIP);
        m_searchLine->setText(QString(strSearch.c_str()));
        m_transferLine->setText(QString(strTransfert.c_str()));
        m_destinationLine->setText(QString(strDestination.c_str()));
        m_backupLine->setText(QString(strBackup.c_str()));
        m_PDFLine->setText(QString(strPDF.c_str()));
        m_IPLine->setText(QString(strIP.c_str()));
    }
}

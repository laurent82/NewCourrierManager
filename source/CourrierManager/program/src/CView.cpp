
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressDialog>
#include <QtCore/QDate>
#include <QHeaderView>


#include "CView.h"
#include "ui_courriermanager.h"

#include "CError.h"
#include "CPatient.h"
#include "CDate.h"

#include "Action/CActionManager.h"
#include "Action/CAction.h"

#include <QDebug>

CView::CView(QWidget *parent)
    : QDialog(parent), ui(new Ui::CourrierManagerClass),
      m_progress( nullptr )
{
    // Ui
    ui->setupUi(this);

    this->setCurrentDate();
    m_panel = new CDrawPanelWidget(0, 0, this);
    this->showMaximized();
    // Ajout des derniers
    ui->m_historyTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->m_historyTable->horizontalHeader()->setStretchLastSection(true);
    ui->m_historyTable->horizontalHeader()->hide();
    ui->m_historyTable->setColumnCount(3);
    ui->m_historyTable->verticalHeader()->hide();

    // Recherche rapide
    m_fastsearch = new CFastSearch();
    connect(m_fastsearch, SIGNAL(finished()), this, SLOT(onTableRefresh()));
    m_modele = new QStringListModel(*m_fastsearch->getCurrentList());
    ui->tablePatient->setModel(m_modele);

    ui->btnConvert->setVisible(false);

    m_tableUsed = false;

    ui->lblConnected->setStyleSheet("color: red;");

    ui->btnSearch->setProperty("commandName", "search");
    ui->btnSend->setProperty("commandName", "send");
    ui->btnDelete->setProperty("commandName", "delete");
    ui->btnValidate->setProperty("commandName", "validate");
    ui->btnNext->setProperty("commandName", "skip");
    ui->btnDelete->setProperty("commandName", "delete");
    ui->btnConnect->setProperty("commandName", "connect");

    ui->btnLastDate->setProperty("type", "small" );
    ui->btnToday->setProperty("type", "small");
    
    style()->unpolish( ui->btnToday );
    style()->polish(ui->btnToday);

    style()->unpolish(ui->btnLastDate);
    style()->polish(ui->btnLastDate);

    connect (ui->btnConfiguration, SIGNAL(clicked()), this, SIGNAL(btnConfigurationClicked()));
    connect (ui->btnSearch, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    connect (ui->btnSend, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    connect (ui->btnValidate, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    connect (ui->btnNext, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    connect (ui->btnDelete, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    connect (ui->btnConnect, SIGNAL(clicked()), this, SLOT(onButtonClicked()));

    ui->btnSend->setEnabled(false);

    // Ajoute la date de la compilation
    ui->lblVersion->setText(QString("Version: ") + QString::fromLocal8Bit(__DATE__));

    connect(CActionManager::instance(), SIGNAL(actionUndone(int)), this, SLOT(onActionUndone(int)));
}


CView::~CView()
{
    delete ui;
    delete m_fastsearch;
    delete m_modele;
}

void CView::resizeEvent(QResizeEvent* event){
    int height = this->height() - 20;
    int width =  (int)(height /1.41);
    m_panel->setPanelSize(width,height);
    ui->toolBar->move(width + 25, 100); //, 540,toolbarWidth);
    ui->topButtons->move(width + 25, 15); // 540, 72);
}

void CView::setProgressBar(int total)
{
    m_progress = new QProgressDialog("Envoi des fichiers...", QString(), 0, total, this, Qt::Popup);
    connect(m_progress, SIGNAL(finished(int)), m_progress, SLOT(deleteLater()));
    m_progress->show();
}

void CView::updateProgressBar(int step)
{
    if (m_progress) {
        m_progress->setValue(m_progress->value() + step);
    }
}

void CView::onInfoReceived(QString key, QVariant value)
{
    if (key.compare("remainingImage") == 0) {
        ui->lblRemaining->setText(QString("%1").arg(value.toInt()));
        if (value.toInt() == 0) {
            displayError(CError::NOMOREFILE);
        } else {

        }
    }

    if (key.compare("remainingPDF") == 0) {
        ui->lblSent->setText(QString("%1").arg(value.toInt()));
    }

    if (key.compare("image") == 0) {
        m_panel->setImage(value.value<QImage>());
    }

    if (key.compare("addToHistory") == 0) {
        if (value.toBool() == true) {
            QString fullName = CPatient::instance()->getParameter("patient_name").toString();
            fullName.append(", ");
            fullName.append(CPatient::instance()->getParameter("patient_surname").toString());
            fullName.append(QString(" (%1)").arg(
                                CPatient::instance()->getParameter("patient_page").toInt()));
            QPushButton* cancel = new QPushButton("X");
            cancel->setProperty("cancel", fullName);
            cancel->setProperty("id", CAction::getLastActionId());
            cancel->setMaximumWidth(30);
            connect(cancel, SIGNAL(clicked()), this, SLOT(onHistoryCancel()));
            ui->m_historyTable->insertRow(0);
            ui->m_historyTable->setCellWidget(0, 0, cancel);
            ui->m_historyTable->setItem(0, 1, new QTableWidgetItem(QString::number(CAction::getLastActionId())));
            ui->m_historyTable->item(0,1)->setTextAlignment(Qt::AlignCenter);
            ui->m_historyTable->setCellWidget(0, 2, new QLabel(fullName));
            ui->m_historyTable->resizeColumnsToContents();

        }
    }
}

void CView::displayError(int errorId)
{
    switch (errorId)
    {
    case CError::NOCONFIGFILE:
    QMessageBox::critical(this, tr("Courrier"),
                             tr("Le fichier de configuration est introuvable.\nLe programme va se terminer."),
                             QMessageBox::Ok);
    break;

    case CError::UNKNOWNDIR:
    QMessageBox::critical(this, tr("Courrier"),
                             tr("Les répertoires sont incorrects. Utilisez l'option de configuration."),
                             QMessageBox::Ok);
    break;

    case CError::NOPATIENT:
    QMessageBox::warning(this, tr("Courrier"),
                             tr("Aucun patient n'a été validé jusqu'à présent."),
                             QMessageBox::Ok);
    break;
    case CError::INVALIDDATE:
    QMessageBox::warning(this, tr("Courrier"),
                             tr("Date invalide. Impossible de retrouver la date."),
                             QMessageBox::Ok);
    break;
    case CError::EMPTYFIELD:
    QMessageBox::critical(this, tr("Courrier"),
                             tr("Un champ est vide..."),
                             QMessageBox::Ok);
    break;

    case CError::RENAME:
        QMessageBox::critical(this, tr("Courrier"),
                              tr("Une erreur est survenue lors de la gestion de ce courrier. Le courrier a-t'il été recherché?"),
                              QMessageBox::Ok);
    break;

    case CError::NOMOREFILE:
        m_panel->setImage(QImage()); // Efface l'image courante.
        QMessageBox::information(this, tr("Courrier"),
                              tr("Il n'y a plus de courrier à classer."),
                              QMessageBox::Ok);
    break;


    case CError::DELETEFILE:
        QMessageBox::critical(this, tr("Courrier"),
                              tr("Erreur lors de la suppression du fichier."),
                              QMessageBox::Ok);
    break;

    case CError::ALLFILESSENT:
        QMessageBox::information(this, tr("Courrier"),
                              tr("Tout le courrier a été envoyé."),
                              QMessageBox::Ok);
    break;

    case CError::NETWORKFILEEXISTS:
        QMessageBox::warning( this, tr("Courrier"),
                              tr("Un fichier existait déjà sur le serveur et n'a pas été copié."),
                              QMessageBox::Ok);
    break;
    case CError::NETWORKPROBLEM:
        if ( m_progress )
        {
            m_progress->hide();
            m_progress->deleteLater();
            m_progress = nullptr;
        }
        QMessageBox::critical(this, tr("Courrier"),
                              tr("Une erreur s'est produite durant le transfert."),
                              QMessageBox::Ok);
    break;
    case CError::NOCONFIGUREDNETWORK:
        QMessageBox::critical(this, tr("Courrier"),
            tr("Le réseau n'a pas été configuré."),
            QMessageBox::Ok);
        break;
    default:
        QMessageBox::information(this, tr("Courrier"),
                              tr("Erreur non répertoriée."),
                              QMessageBox::Ok);
    break;
    }
}

void CView::setCurrentDate(){
    QDate date = QDate::currentDate();
    ui->txtDay->setText(QString("%1").arg(date.day()));
    ui->txtMonth->setText(QString("%1").arg(date.month()));
    ui->txtYear->setText(QString("%1").arg(date.year()));
}

void CView::resetInfoPatient()
{
    ui->txtName->clear();
    ui->txtSurname->clear();
    // Date du jour
    this->setCurrentDate();
    ui->spPage->setValue(1);
    m_tableUsed = false;
}

bool CView::checkFields()
{
    if ( ui->txtName->text().isEmpty() ||
         ui->txtSurname->text().isEmpty() ||
         ui->txtDay->text().isEmpty() ||
         ui->txtMonth->text().isEmpty() ||
         ui->txtYear->text().isEmpty()
         ) {
        return false;
    } else {
        return true;
    }
}

void CView::removeFromHistory(int id)
{
    // Action réussie. Enlever l'élément dans la liste
    for (int i = 0; i < ui->m_historyTable->rowCount(); ++i) {
        int currentId = ui->m_historyTable->item(i, 1)->text().toInt();
        if (currentId == id){
             ui->m_historyTable->removeRow(i);
             return;
        }
    }
}


void CView::clearFields()
{
    ui->txtName->setText("");
    ui->txtSurname->setText("");
    ui->spPage->setValue(1);
    setCurrentDate();
}

void CView::fillPatient()
{
    CPatient::instance()->configure("patient_name",  ui->txtName->text());
    CPatient::instance()->configure("patient_surname",  ui->txtSurname->text());
    CPatient::instance()->configure("patient_date", constructDate());
    CPatient::instance()->configure("patient_page", ui->spPage->value());
    CPatient::instance()->configure("patient_table", m_tableUsed);

}


QString CView::constructDate()
{
    QString date;
    int iDay = ui->txtDay->text().toInt();
    int iMonth = ui->txtMonth->text().toInt();
    int iYear = ui->txtYear->text().toInt();
    if (iDay < 1 || iDay > 31 || iMonth < 1 || iMonth > 12 || iYear < 1950 ){
        displayError(CError::INVALIDDATE);
        return QString();
    }
    date = ui->txtYear->text();
    if (iMonth< 10)
        date.append("0");
    date.append(QString("%1").arg(iMonth));
    if (iDay < 10)
        date.append("0");
    date.append(QString("%1").arg(iDay));
    return date;
}


void CView::onButtonClicked()
{
    QString name = sender()->property("commandName").toString();

    // Cas du bouton Valider
    if (name.compare("validate") == 0) {
        if (checkFields()) {
            fillPatient();
            clearFields();
            if (!m_tableUsed) {
                m_fastsearch->appendNewPatient();
            }
        } else {
            displayError(CError::EMPTYFIELD);
            return;
        }
    }
    emit sendCommand(name);
}

void CView::onHistoryCancel()
{
    int currentId = sender()->property("id").toInt();
    bool actionRep = CActionManager::instance()->cancelActionById(currentId);
    if (actionRep) {
        removeFromHistory(currentId);
        emit sendCommand("search");

    }
}

void CView::on_btnToday_clicked(){
    setCurrentDate();
}

void CView::on_btnLastDate_clicked()
{
    QString day, month, year;
    if (!m_lastDate.isEmpty()){
        CDate::extractDate(m_lastDate, day, month, year);
        ui->txtYear->setText(year);
        ui->txtMonth->setText(month);
        ui->txtDay->setText(day);
    }
    else{
        setCurrentDate();
    }
}

void CView::on_btnSamePatient_clicked()
{
    QString name, surname, date;
    QString day, month, year;
    int page;

    name = CPatient::instance()->getParameter("patient_name").toString();
    surname = CPatient::instance()->getParameter("patient_surname").toString();
    date = CPatient::instance()->getParameter("patient_date").toString();
    page = CPatient::instance()->getParameter("patient_page").toInt();
    if (name.isEmpty() && date.isEmpty()){
        displayError(CError::NOPATIENT);
        return;
    }
    ui->txtName->setText(name);
    ui->txtSurname->setText(surname);
    CDate::extractDate(date, day, month, year);
    ui->txtYear->setText(year);
    ui->txtMonth->setText(month);
    ui->txtDay->setText(day);
    ui->spPage->setValue(page);
}

void CView::on_txtName_textEdited(QString _txt){
    if (ui->cbFastSearch->isChecked()){
        m_fastsearch->setWord(_txt.toUpper());
        m_fastsearch->start();
    }
    CPatient::instance()->configure("patient_table", false);
}

void CView::on_txtSurname_textEdited(QString _txt){
    CPatient::instance()->configure("patient_table", false);
}

void CView::on_tablePatient_clicked(){
    int i = ui->tablePatient->currentIndex().row();
    QString str;
    m_fastsearch->getItem(i, str);
    ui->txtName->setText(str.section(',',0,0).trimmed());
    ui->txtSurname->setText(str.section(",", 1,1).trimmed());
    CPatient::instance()->configure("patient_table", true);
}

void CView::onConnectedToHost()
{
    ui->btnSend->setEnabled(true);
    ui->lblConnected->setStyleSheet("color: black;");
    ui->lblConnected->setText("Connecté");
    ui->btnConnect->setVisible(false);
}

void CView::onDisconnectedFromHost()
{
    ui->btnSend->setEnabled(false);
    ui->lblConnected->setStyleSheet("color: red;");
    ui->lblConnected->setText("Non connecté");
    ui->btnConnect->setVisible(true);
}

void CView::onActionUndone(int id)
{
    removeFromHistory(id);
}

void CView::onTableRefresh()
{
    m_modele->setStringList(*m_fastsearch->getCurrentList());
}

void CView::updateNetworkMethod()
{
    QSettings settings;
    QString current_method = settings.value("network_method").toString();
    if ( current_method.compare("server") == 0 )
    {
        ui->serverBar->setVisible( true );
    }
    else
    {
        ui->serverBar->setVisible( false );
        ui->btnSend->setEnabled( true );
    }
}

QStringList* CView::getPatientList() 
{
    return m_fastsearch->getFullList();
}



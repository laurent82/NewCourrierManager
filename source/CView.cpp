#include "CView.h"
#include "ui_courriermanager.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QDate>
#include <QMessageBox>
#include <QProgressDialog>
#include <QPlastiqueStyle>
#include <QCleanlooksStyle>

#include "CError.h"
#include "CPatient.h"
#include "CDate.h"

#include <QDebug>

CView::CView(QWidget *parent)
    : QDialog(parent), ui(new Ui::CourrierManagerClass),
      m_progress(0)
{
    // Ui
    QApplication::setStyle(new QPlastiqueStyle/*QCleanlooksStyle*/);
    ui->setupUi(this);
    this->setCurrentDate();
    m_panel = new CDrawPanelWidget(0, 0, this);
   // this->showMaximized();

    // Ajout des derniers
    m_history = new QStringList;
    m_historyModel = new QStringListModel(*m_history);
    ui->tableLast->setModel(m_historyModel);

    // Recherche rapide
    m_fastsearch = new CFastSearch();
    m_modele = new QStringListModel(*m_fastsearch->getCurrentList());
    ui->tablePatient->setModel(m_modele);

    ui->btnBack->setVisible(false);
   // ui->btnSend->setVisible(false);
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
}


CView::~CView()
{
    delete ui;
    delete m_fastsearch;
    delete m_modele;
    delete m_history;
    delete m_historyModel;
}

void CView::resizeEvent(QResizeEvent* event){
    int height = this->height() - 20;
    int width =  (int)(height /1.41);
    m_panel->setPanelSize(width,height);
    int toolbarWidth = 680;
    ui->toolBar->move(width + 25, 100); //, 540,toolbarWidth);
    ui->topButtons->move(width + 25, 15); // 540, 72);
    // this->setMaximumWidth(width + toolbarWidth + 110);
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
            fullName.append(QString(" (%1 - %2)").arg(
                                CPatient::instance()->getParameter("patient_page").toInt()).arg(
                                CPatient::instance()->getParameter("patient_date").toString()));
            m_history->prepend(fullName);
            m_historyModel->setStringList(*m_history);
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

    case CError::NETWORKPROBLEM:
        QMessageBox::critical(this, tr("Courrier"),
                              tr("Une erreur s'est produite durant le transfert."),
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
        m_modele->setStringList(*m_fastsearch->getCurrentList());
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

//void CView::onRefreshRemaining(int iRemaining, int iSendRemaining)
//{
//    ui->lblRemaining->setText(QString("%1").arg(iRemaining));
//    ui->lblSent->setText(QString("%1").arg(iSendRemaining));
//}


//void CView::onPrepareNext(const QString& str)
//{
//    resetInfoPatient();
//    //  QString str;
//    //  m_gestion->getNextFile(str);
//    if (str.isNull() || str.isEmpty()) {
//        QMessageBox::information(this, tr("Courrier"),
//                                 tr("Il n'y a plus de courrier à classer!"),
//                                 QMessageBox::Ok);
//    }
//    else {
//        if (!m_panel->setFile(str)) {
//            QMessageBox::information(this, tr("Courrier"),
//                                     tr("Une erreur est survenue lors du chargement de l'image."),
//                                     QMessageBox::Ok);
//        }
//    }
//    //   refreshRemaining();
//}

//void CView::onSetFile(QString str)
//{
//    /*refreshRemaining();
//    if (!str.isNull()) {
//        m_panel->setFile(str);
//    }
//    else {
//        QMessageBox::information(this, tr("Courrier"),
//                                 tr("Il n'y a plus de courrier à classer!"),
//                                 QMessageBox::Ok);
//    }
//    */
//}

//void CView::on_btnValidate_clicked()
//{

//    }
//}










//void CView::on_btnSend_clicked(){
//    /*
//    m_Copying = true;
//    QProgressDialog pd ("Copie du courrier sur le serveur en cours...", "Annuler", 0, 100);
//    //   pd.forceShow();
//    connect(&pd, SIGNAL(canceled()), this, SLOT(copyCancel()));
//    int iRemaining, iSendRemaining;
//    //float step = 100/(float)iSendRemaining;
//    m_gestion->initCopyFile();
//    m_gestion->getRemaining(iRemaining, iSendRemaining);
//    pd.setRange(0,iSendRemaining);
//    int i = 0;
//    for (; i < iSendRemaining && m_Copying; ++i){
//        if (!m_gestion->copyNextFile()){
//            m_Copying = false;
//            QMessageBox::critical(this, tr("Courrier"),
//                                  tr("Une erreur est survenue lors de la copie des fichiers."),
//                                  QMessageBox::Ok);
//        }
//        else{
//           pd.setValue(i);
//        }
//    }
//    // delete pd;
//    if (iSendRemaining-i == 0)
//        QMessageBox::information(this, tr("Courrier"),
//                                 tr("Tout le courrier classé a été copié sur le serveur!"),
//                                 QMessageBox::Ok);
//    ui->lblSent->setText(QString("%1").arg(iSendRemaining-i));
//    */
//}


//void CView::copyCancel() {
//    m_Copying = false;
//}

//void CView::on_btnNext_clicked(){
//    //prepareNext();
//}

//void CView::on_btnDelete_clicked()
//{
//    int rep = QMessageBox::question(this, tr("Courrier"),
//                                    tr("Ce fichier sera supprimé définitivement. Continuer?"),
//                                    QMessageBox::Yes|QMessageBox::No);
//    if (rep == QMessageBox::Yes) {
//        emit deleteFile();
//    }
//    /*
//    if (rep == QMessageBox::Yes){
//        if (m_gestion->deleteFile())
//            prepareNext();
//        else
//            QMessageBox::warning(this, tr("Courrier"),
//                                 tr("Une erreur est survenue lors de la suppression de ce fichier."),
//                                 QMessageBox::Ok);
//    }*/
//}

//void CView::on_btnConvert_clicked(){
//    /*
//    int rep = QMessageBox::question(this, tr("Courrier"),
//                                    tr("Attention! Cette opération est très longue!\nVoulez-vous continuer?"),
//                                    QMessageBox::Yes|QMessageBox::No);
//    if (rep == QMessageBox::Yes){
//        m_gestion->convertPDFall();
//    }
//    */
//}



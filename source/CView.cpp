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


CView::CView(QWidget *parent)
    : QDialog(parent), ui(new Ui::CourrierManagerClass)
{
    // Ui
    QApplication::setStyle(new QPlastiqueStyle/*QCleanlooksStyle*/);
    ui->setupUi(this);
    this->setCurrentDate();
    m_panel = new CDrawPanelWidget(0, 0, this);
   // this->showMaximized();

    // Ajout des derniers
    m_lastAdded = new QStringList;
    m_lastAddedModel = new QStringListModel(*m_lastAdded);
    ui->tableLast->setModel(m_lastAddedModel);

    // Recherche rapide
    m_fastsearch = new CFastSearch();
    m_modele = new QStringListModel(*m_fastsearch->getCurrentList());
    ui->tablePatient->setModel(m_modele);

/*
    m_gestion = new CGestion();
    m_gestion->setFastSearch(m_fastsearch);
    m_gestion->setLastAdded(m_lastAdded);
*/
    ui->btnBack->setVisible(false);
   // ui->btnSend->setVisible(false);
    ui->btnConvert->setVisible(false);

    m_tableUsed = false;

    ui->btnSearch->setProperty("commandName", "search");
    ui->btnDelete->setProperty("commandName", "delete");

    connect (ui->btnConfiguration, SIGNAL(clicked()), this, SIGNAL(btnConfigurationClicked()));
    connect (ui->btnSearch, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    // Ajoute la date de la compilation
    ui->lblVersion->setText(QString("Version: ") + QString::fromLocal8Bit(__DATE__));
}


CView::~CView()
{
    delete ui;
    delete m_fastsearch;
    delete m_modele;
    delete m_lastAdded;
    delete m_lastAddedModel;
}


void CView::resizeEvent(QResizeEvent* event){
    int height = this->height() - 20;
    int width =  (int)(height /1.41);
    m_panel->setPanelSize(width,height);
    int toolbarWidth = 680;
    ui->toolBar->setGeometry(width + 25, 100, 540,toolbarWidth);
    ui->topButtons->setGeometry(width + 25, 15, 540, 72);
    // this->setMaximumWidth(width + toolbarWidth + 110);
}

void CView::onInfoReceived(QString key, QVariant value)
{
    if (key.compare("remainingImage") == 0) {
        ui->lblRemaining->setText(QString("%1").arg(value.toInt()));
    }

    if (key.compare("remainingPDF") == 0) {
        ui->lblSent->setText(QString("%1").arg(value.toInt()));
    }

    if (key.compare("image") == 0) {
        m_panel->setImage(value.value<QImage>());
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
                             tr("Les r�pertoires sont incorrects. Utilisez l'option de configuration."),
                             QMessageBox::Ok);
    break;
    }
}



bool CView::constructDate(QString &_date){
    int iDay = ui->txtDay->text().toInt();
    int iMonth = ui->txtMonth->text().toInt();
    int iYear = ui->txtYear->text().toInt();
    if (iDay < 1 || iDay > 31 || iMonth < 1 || iMonth > 12 || iYear < 1950 ){
        QMessageBox::warning(this, tr("Courrier"),
                             tr("La date n'a pas un format correct."),
                             QMessageBox::Ok);
        return false;
    }
    _date = ui->txtYear->text();
    if (iMonth< 10)
        _date.append("0");
    _date.append(QString("%1").arg(iMonth));
    if (iDay < 10)
        _date.append("0");
    _date.append(QString("%1").arg(iDay));
    return true;
}

void CView::setCurrentDate(){
    QDate date = QDate::currentDate();
    ui->txtDay->setText(QString("%1").arg(date.day()));
    ui->txtMonth->setText(QString("%1").arg(date.month()));
    ui->txtYear->setText(QString("%1").arg(date.year()));
}

void CView::resetInfoPatient(){
    ui->txtName->clear();
    ui->txtSurname->clear();
    // Date du jour
    this->setCurrentDate();
    ui->spPage->setValue(1);
    m_tableUsed = false;
}

void CView::onButtonClicked()
{
    QString name = sender()->property("commandName").toString();
    emit sendCommand(name);
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
//                                 tr("Il n'y a plus de courrier � classer!"),
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
//                                 tr("Il n'y a plus de courrier � classer!"),
//                                 QMessageBox::Ok);
//    }
//    */
//}

//void CView::on_btnValidate_clicked()
//{
//    // R�cup�ration des infos des champs.
//    QString date;
//    if (constructDate(date)){
//        if (ui->txtName->text().isEmpty() || ui->txtSurname->text().isEmpty() || ui->txtDay->text().isEmpty()
//            ||  ui->txtMonth->text().isEmpty() || ui->txtYear->text().isEmpty() ){
//            QMessageBox::warning(this, tr("Courrier"),
//                                 tr("Un des champs est vide..."),
//                                 QMessageBox::Ok);
//            return;
//        }
//        CPatient::instance()->clear();
//        CPatient::instance()->configure("patient_name", QVariant(ui->txtName->text().toUpper()));
//        CPatient::instance()->configure("patient_surname", QVariant(ui->txtSurname->text().toLower()));
//        CPatient::instance()->configure("patient_date", QVariant(date));
//        CPatient::instance()->configure("patient_page", QVariant(ui->spPage->value()));
//        emit btnValidateClicked();

//        /*
//        if (!m_gestion->renameFile(m_tableUsed)){
//            QMessageBox::critical(this, tr("Courrier"),
//                                  tr("Une erreur est survenue lors de la gestion de ce courrier. Le courrier a-t'il �t� recherch�?"),
//                                  QMessageBox::Ok);
//        }
//        else{
//            prepareNext();
//            m_lastAddedModel->setStringList(*m_lastAdded);
//        }
//        */
//    }
//}


//void CView::on_btnSamePatient_clicked(){
//    /*
//    QString name, surname, date;
//    QString day, month, year;
//    int page;
//    m_gestion->getInfo(name, surname, date, page);
//    if (name.isEmpty() && date.isEmpty()){
//        // TODO : Msg erreur, pas encore eu de patient
//        return;
//    }
//    ui->txtName->setText(name);
//    ui->txtSurname->setText(surname);
//    m_gestion->extractDate(date, day, month, year);
//    ui->txtYear->setText(year);
//    ui->txtMonth->setText(month);
//    ui->txtDay->setText(day);
//    ui->spPage->setValue(page);
//    */
//}

//void CView::on_btnToday_clicked(){
//    setCurrentDate();
//}

//void CView::on_btnLastDate_clicked(){
//    /*
//    QString date, day, month, year;
//    m_gestion->getDate(date);
//    if (!date.isEmpty()){
//        m_gestion->extractDate(date, day, month, year);
//        ui->txtYear->setText(year);
//        ui->txtMonth->setText(month);
//        ui->txtDay->setText(day);
//    }
//    else{
//        setCurrentDate();
//    }
//    */
//}

//void CView::on_txtName_textEdited(QString _txt){
//    if (ui->cbFastSearch->isChecked()){
//        m_fastsearch->setWord(_txt.toUpper());
//        m_fastsearch->run();
//        m_modele->setStringList(*m_fastsearch->getCurrentList());
//    }
//    m_tableUsed = false;
//}

//void CView::on_txtSurname_textEdited(QString _txt){
//    m_tableUsed = false;
//}

//void CView::on_tablePatient_clicked(){
//    int i = ui->tablePatient->currentIndex().row();
//    QString str;
//    m_fastsearch->getItem(i, str);
//    ui->txtName->setText(str.section(',',0,0));
//    ui->txtSurname->setText(str.section(", ", 1,1));
//    m_tableUsed = true;
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
//                                 tr("Tout le courrier class� a �t� copi� sur le serveur!"),
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
//                                    tr("Ce fichier sera supprim� d�finitivement. Continuer?"),
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
//                                    tr("Attention! Cette op�ration est tr�s longue!\nVoulez-vous continuer?"),
//                                    QMessageBox::Yes|QMessageBox::No);
//    if (rep == QMessageBox::Yes){
//        m_gestion->convertPDFall();
//    }
//    */
//}


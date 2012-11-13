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
    ui->btnValidate->setProperty("commandName", "validate");

    connect (ui->btnConfiguration, SIGNAL(clicked()), this, SIGNAL(btnConfigurationClicked()));
    connect (ui->btnSearch, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    connect (ui->btnValidate, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
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

    if (key.compare("addToTable") == 0) {
        if (value.toBool() == true) {
            QString fullName = CPatient::instance()->getParameter("patient_name").toString();
            fullName.append(", ");
            fullName.append(CPatient::instance()->getParameter("patient_surname").toString());
            fullName.append(QString(" (%1 - %2)").arg(
                                CPatient::instance()->getParameter("patient_page").toInt()).arg(
                                CPatient::instance()->getParameter("patient_date").toString()));
            m_lastAdded->prepend(fullName);
            m_lastAddedModel->setStringList(*m_lastAdded);
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
                             tr("Les r�pertoires sont incorrects. Utilisez l'option de configuration."),
                             QMessageBox::Ok);
    break;

    case CError::NOPATIENT:
    QMessageBox::warning(this, tr("Courrier"),
                             tr("Aucun patient n'a �t� valid� jusqu'� pr�sent."),
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
                              tr("Une erreur est survenue lors de la gestion de ce courrier. Le courrier a-t'il �t� recherch�?"),
                              QMessageBox::Ok);
    break;

    case CError::NOMOREFILE:
        QMessageBox::information(this, tr("Courrier"),
                              tr("Il n'y a plus de courrier � classer."),
                              QMessageBox::Ok);
    break;
    }
}



bool CView::constructDate(QString &_date){
    int iDay = ui->txtDay->text().toInt();
    int iMonth = ui->txtMonth->text().toInt();
    int iYear = ui->txtYear->text().toInt();
    if (iDay < 1 || iDay > 31 || iMonth < 1 || iMonth > 12 || iYear < 1950 ){
        displayError(CError::INVALIDDATE);
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

void CView::extractDate(const QString &date, QString& day, QString& month, QString& year)
{
    QChar c;
    QString var = "";

    // R�cup�re l'ann�e
    for (int i = 0; i < 4; ++i){
        c = date.at(i);
        var.append(c);
    }
    year = var;
    // R�cup�re le mois
    var.clear();
    c = date.at(4);
    var.append(c);
    c = date.at(5);
    var.append(c);
    month = var;
    // R�cup�re le jour
    var.clear();
    c = date.at(6);
    var.append(c);
    c = date.at(7);
    var.append(c);
    day = var;
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

void CView::onButtonClicked()
{
    QString name = sender()->property("commandName").toString();

    // Cas du bouton Valider
    if (name.compare("validate") == 0) {
        if (!checkFields()) {
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
        extractDate(m_lastDate, day, month, year);
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
    extractDate(date, day, month, year);
    ui->txtYear->setText(year);
    ui->txtMonth->setText(month);
    ui->txtDay->setText(day);
    ui->spPage->setValue(page);
}

void CView::on_txtName_textEdited(QString _txt){
    if (ui->cbFastSearch->isChecked()){
        m_fastsearch->setWord(_txt.toUpper());
        m_fastsearch->run();
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
    ui->txtName->setText(str.section(',',0,0));
    ui->txtSurname->setText(str.section(", ", 1,1));
    CPatient::instance()->configure("patient_table", true);
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



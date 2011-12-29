#include "courriermanager.h"
#include "ui_courriermanager.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QDate>
#include <QMessageBox>
#include <QProgressDialog>
#include <QPlastiqueStyle>
#include <QCleanlooksStyle>


CourrierManager::CourrierManager(QWidget *parent)
    : QDialog(parent), ui(new Ui::CourrierManagerClass)
{
    QApplication::setStyle(new QPlastiqueStyle/*QCleanlooksStyle*/);
    ui->setupUi(this);
    this->setCurrentDate();
    panel = new CDrawPanel(0, 0, this);
    this->showMaximized();
    m_lastAdded = new QStringList;
    m_lastAddedModel = new QStringListModel(*m_lastAdded);
    ui->tableLast->setModel(m_lastAddedModel);
    m_fastsearch = new CFastSearch();
    m_modele = new QStringListModel(*m_fastsearch->getCurrentList());
    ui->tablePatient->setModel(m_modele);
    gestion = new CGestion();
    gestion->setFastSearch(m_fastsearch);
    gestion->setLastAdded(m_lastAdded);

    ui->btnBack->setVisible(false);
   // ui->btnSend->setVisible(false);
    ui->btnConvert->setVisible(false);


    m_tableUsed = false;

}

CourrierManager::~CourrierManager()
{
    delete ui;
    delete gestion;
    delete m_fastsearch;
    delete m_modele;
    delete m_lastAdded;
    delete m_lastAddedModel;
}


void CourrierManager::resizeEvent(QResizeEvent* event){
    int height = this->height() - 20;
    int width =  (int)(height /1.41);
    panel->setPanelSize(width,height);
    int toolbarWidth = 680;
    ui->toolBar->setGeometry(width + 25, 100, 540,toolbarWidth);
    ui->topButtons->setGeometry(width + 25, 15, 540, 72);
    // this->setMaximumWidth(width + toolbarWidth + 110);
}

void CourrierManager::refreshRemaining(){
    int iRemaining, iSendRemaining;
    gestion->getRemaining(iRemaining, iSendRemaining);
    ui->lblRemaining->setText(QString("%1").arg(iRemaining));
    ui->lblSent->setText(QString("%1").arg(iSendRemaining));
}

void CourrierManager::prepareNext(){
    resetInfoPatient();
    QString str;
    gestion->getNextFile(str);
    if (str.isNull() || str.isEmpty()){
        QMessageBox::information(this, tr("Courrier"),
                                 tr("Il n'y a plus de courrier à classer!"),
                                 QMessageBox::Ok);
    }
    if (!panel->setFile(str)){
        QMessageBox::information(this, tr("Courrier"),
                                 tr("Une erreur est survenue lors du chargement de l'image."),
                                 QMessageBox::Ok);
    }
    refreshRemaining();
}

bool CourrierManager::constructDate(QString &_date){
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

void CourrierManager::setCurrentDate(){
    QDate date = QDate::currentDate();
    ui->txtDay->setText(QString("%1").arg(date.day()));
    ui->txtMonth->setText(QString("%1").arg(date.month()));
    ui->txtYear->setText(QString("%1").arg(date.year()));
}

void CourrierManager::resetInfoPatient(){
    ui->txtName->clear();
    ui->txtSurname->clear();
    // Date du jour
    this->setCurrentDate();
    ui->spPage->setValue(1);
    m_tableUsed = false;
}



void CourrierManager::on_btnSearch_clicked(){
    gestion->search();
    this->refreshRemaining();
    QString str;
    gestion->getFile(str);
    if (!str.isNull())
        panel->setFile(str);
    else
        QMessageBox::information(this, tr("Courrier"),
                                 tr("Il n'y a plus de courrier à classer!"),
                                 QMessageBox::Ok);
}

void CourrierManager::on_btnValidate_clicked(){
    QString date;
    if (constructDate(date)){
        if (ui->txtName->text().isEmpty() || ui->txtSurname->text().isEmpty() || ui->txtDay->text().isEmpty()
            ||  ui->txtMonth->text().isEmpty() || ui->txtYear->text().isEmpty() ){
            QMessageBox::warning(this, tr("Courrier"),
                                 tr("Un des champs est vide..."),
                                 QMessageBox::Ok);
            return;
        }
        gestion->setInfo(ui->txtName->text().toUpper(), ui->txtSurname->text().toLower(), date, ui->spPage->value());
        if (!gestion->renameFile(m_tableUsed)){
            QMessageBox::critical(this, tr("Courrier"),
                                  tr("Une erreur est survenue lors de la gestion de ce courrier. Le courrier a-t'il été recherché?"),
                                  QMessageBox::Ok);
        }
        else{
            prepareNext();
            m_lastAddedModel->setStringList(*m_lastAdded);
        }
    }
}


void CourrierManager::on_btnSamePatient_clicked(){
    QString name, surname, date;
    QString day, month, year;
    int page;
    gestion->getInfo(name, surname, date, page);
    if (name.isEmpty() && date.isEmpty()){
        // TODO : Msg erreur, pas encore eu de patient
        return;
    }
    ui->txtName->setText(name);
    ui->txtSurname->setText(surname);
    gestion->extractDate(date, day, month, year);
    ui->txtYear->setText(year);
    ui->txtMonth->setText(month);
    ui->txtDay->setText(day);
    ui->spPage->setValue(page);
}

void CourrierManager::on_btnToday_clicked(){
    setCurrentDate();
}

void CourrierManager::on_btnLastDate_clicked(){
    QString date, day, month, year;
    gestion->getDate(date);
    if (!date.isEmpty()){
        gestion->extractDate(date, day, month, year);
        ui->txtYear->setText(year);
        ui->txtMonth->setText(month);
        ui->txtDay->setText(day);
    }
    else{
        setCurrentDate();
    }
}

void CourrierManager::on_txtName_textEdited(QString _txt){
    if (ui->cbFastSearch->isChecked()){
        m_fastsearch->setWord(_txt.toUpper());
        m_fastsearch->run();
        m_modele->setStringList(*m_fastsearch->getCurrentList());
    }
    m_tableUsed = false;
}

void CourrierManager::on_txtSurname_textEdited(QString _txt){
    m_tableUsed = false;
}

void CourrierManager::on_tablePatient_clicked(){
    int i = ui->tablePatient->currentIndex().row();
    QString str;
    m_fastsearch->getItem(i, str);
    ui->txtName->setText(str.section(',',0,0));
    ui->txtSurname->setText(str.section(", ", 1,1));
    m_tableUsed = true;
}

void CourrierManager::on_btnSend_clicked(){
    m_Copying = true;
    QProgressDialog pd ("Copie du courrier sur le serveur en cours...", "Annuler", 0, 100);
    //   pd.forceShow();
    connect(&pd, SIGNAL(canceled()), this, SLOT(copyCancel()));
    int iRemaining, iSendRemaining;
    //float step = 100/(float)iSendRemaining;
    gestion->initCopyFile();
    gestion->getRemaining(iRemaining, iSendRemaining);
    pd.setRange(0,iSendRemaining);
    int i = 0;
    for (; i < iSendRemaining && m_Copying; ++i){
        if (!gestion->copyNextFile()){
            m_Copying = false;
            QMessageBox::critical(this, tr("Courrier"),
                                  tr("Une erreur est survenue lors de la copie des fichiers."),
                                  QMessageBox::Ok);
        }
        else{
           pd.setValue(i);
        }
    }
    // delete pd;
    if (iSendRemaining-i == 0)
        QMessageBox::information(this, tr("Courrier"),
                                 tr("Tout le courrier classé a été copié sur le serveur!"),
                                 QMessageBox::Ok);
    ui->lblSent->setText(QString("%1").arg(iSendRemaining-i));
}


void CourrierManager::copyCancel() {
    m_Copying = false;
}

void CourrierManager::on_btnNext_clicked(){
    prepareNext();
}

void CourrierManager::on_btnDelete_clicked(){
    int rep = QMessageBox::question(this, tr("Courrier"),
                                    tr("Ce fichier sera supprimé définitivement. Continuer?"),
                                    QMessageBox::Yes|QMessageBox::No);
    if (rep == QMessageBox::Yes){
        if (gestion->deleteFile())
            prepareNext();
        else
            QMessageBox::warning(this, tr("Courrier"),
                                 tr("Une erreur est survenue lors de la suppression de ce fichier."),
                                 QMessageBox::Ok);
    }
}

void CourrierManager::on_btnConvert_clicked(){
    int rep = QMessageBox::question(this, tr("Courrier"),
                                    tr("Attention! Cette opération est très longue!\nVoulez-vous continuer?"),
                                    QMessageBox::Yes|QMessageBox::No);
    if (rep == QMessageBox::Yes){
        gestion->convertPDFall();
    }
}



#include "CGestion.h"

#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <QProgressDialog>
#include <QVariant>

#include "CError.h"

// Lecture, écriture de fichier
// A remplacer avec les fonctions Qt ?
#include <fstream>
#include <string>
using namespace std;



CGestion::CGestion()
{}

CGestion::~CGestion(){
    delete m_fileList;
    delete m_fileToCopyList;
}

void CGestion::loadConfigFile()
{
    // Lecture des paramètres
    ifstream file("config.txt");
    if (file) {
    string strSearch, strTransfert, strDestination, strBackup, strPDF;
    getline(file, strSearch);
    getline(file, strTransfert);
    getline(file, strDestination);
    getline(file, strBackup);
    getline(file, strPDF);
    m_fileList = new QStringList();
    m_fileToCopyList = 0;
    m_searchDir = QString(strSearch.c_str());
    m_transferDir = QString(strTransfert.c_str());
    m_destinationDir = QString(strDestination.c_str());
    m_backupDir = QString(strBackup.c_str());
    m_PDFDir = QString(strPDF.c_str());
    m_sendremaining = 0;
    }
    else {
        emit errorOccur(CError::NOCONFIGFILE);
    }
}

void CGestion::onSearch()
{
    m_fileList->clear();
    QDir dir(m_searchDir);
    QStringList filters;
    filters << "*.jpg" << "*.JPG";
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        m_fileList->append(fileInfo.fileName());
    }
    dir = QDir(m_transferDir);
    QStringList pdfFilters;
    pdfFilters << "*.pdf" << "*.PDF";
    dir.setNameFilters(pdfFilters);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    m_sendremaining = dir.entryInfoList().size();
    m_i = 0;
}

void CGestion::onValidate(CPatient* patient)
{

}

/*
void CGestion::refreshRemaining()
{
    int iRemaining = (m_fileList->size() - m_i >= 0)? m_fileList->size() : 0;
    int iSendRemaining = m_sendremaining;
    emit sentRefreshRemaining(iRemaining, iSendRemaining);
}
*/

void CGestion::getNextFile(QString& _resp){
    m_i++;
    getFile(_resp);
}

void CGestion::getFile(QString& _resp){
    if (!m_fileList->isEmpty()){
        if (m_i >= 0 && m_i < m_fileList->size())
            _resp = m_searchDir + m_fileList->at(m_i);
    }
}

void CGestion::setInfo(QString _name, QString _surname, QString _date, int _page){
  /*
    m_lastPatient->name = _name;
    m_lastPatient->surname = _surname;
    m_lastPatient->date = _date;
    m_lastPatient->page = _page;
    */
}

void CGestion::getInfo(QString& _name, QString& _surname, QString& _date, int& _page){
   /*
    m_lastPatient->page++;
    _name = m_lastPatient->name;
    _surname = m_lastPatient->surname;
    _date = m_lastPatient->date;
    _page = m_lastPatient->page;
  */
}

void CGestion::setFastSearch(CFastSearch* _fs){
   m_fastsearch = _fs;
}

void CGestion::setLastAdded(QStringList* _la){
    m_lastAdded = _la;
}

void CGestion::getDate(QString& _date){
    _date = m_lastPatient->date;
}

QString CGestion::constructFileName(CPatient* patient, int type)
{
    QString fileName = QString();
    switch(type){
    case TYPE_JPG:
        fileName = patient->getParameter("data").toString();
        fileName.append("_");
        fileName.append(patient->getParameter("name").toString());
        fileName.append("_");
        fileName.append(patient->getParameter("surname").toString());
        fileName.append("_");
        fileName.append(patient->getParameter("page").toString());
        fileName.append(".jpg");
        break;
    case TYPE_PDF:
        fileName = patient->getParameter("name").toString();
        fileName.append("_");
        fileName.append(patient->getParameter("surname").toString().toUpper());
        fileName.append("_$_");
        // Conversion de la date au format JJMMAAAA
        QString day, month, year;
        extractDate(patient->getParameter("date").toString(), day, month, year);
        fileName.append(day);
        fileName.append(month);
        fileName.append(year);
        fileName.append("_RESUMECONTACT.pdf");
    }
    return fileName;
}

void CGestion::extractDate(const QString &date, QString& day, QString& month, QString& year){
    QChar c;
    QString var = "";

    // Récupère l'année
    for (int i = 0; i < 4; ++i){
        c = date.at(i);
        var.append(c);
    }
    year = var;
    // Récupère le mois
    var.clear();
    c = date.at(4);
    var.append(c);
    c = date.at(5);
    var.append(c);
    month = var;
    // Récupère le jour
    var.clear();
    c = date.at(6);
    var.append(c);
    c = date.at(7);
    var.append(c);
    day = var;
}

bool CGestion::convertPDF(QString _backupFileName){
    // Création du nouveau nom
    QString strFileNamePDF;
    QString strTransferNamePDF;
    constructFileName(strFileNamePDF, TYPE_PDF);
    strTransferNamePDF = m_transferDir + strFileNamePDF;
    strFileNamePDF.prepend(m_PDFDir);
    if (!QFile::exists( strFileNamePDF)){
        QProcess::execute(QString("convert.exe %1 %2").arg(_backupFileName).arg(strFileNamePDF));
    }
    else{
        // Conversion de l'image en PDF
        QString tempName = m_PDFDir + "temp.pdf";
        QProcess::execute(QString("convert.exe %1 %2").arg(_backupFileName).arg(tempName));
        // Renommage du PDF originale
        QString startName = m_PDFDir + "start.pdf";
        QFile file(strFileNamePDF);
        file.rename(startName);
        // Concat à l'aide de PDFTK
        QProcess::execute(QString("pdftk.exe %1 %2 cat output %3").arg(startName).arg(tempName).arg(strFileNamePDF));
        // Suppression des fichiers temporaires
        QFile file2(tempName);
        QFile file3(startName);
        file2.remove();
        file3.remove();
    }
    // Ajout dans le répertoire toTransfer
    // Suppression de l'ancienne version si elle existe
    if (QFile::exists( strTransferNamePDF)){
        QFile file(strTransferNamePDF);
        file.remove();
    }
    // Copie du fichier
    QFile file4(strFileNamePDF);
    file4.copy(strTransferNamePDF);


    return true;
}

void CGestion::convertPDFall(){

    // Récupération de la liste des fichiers à copier
    QDir dir(m_backupDir);
    QStringList filters;
    QStringList* filesToConvert = new QStringList;
    filters << "*.jpg" << "*.JPG";
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        filesToConvert->append(fileInfo.fileName());
    }
    QProgressDialog pd ("Conversion en cours...", "Annuler", 0, filesToConvert->size());
    pd.setAutoClose(false);
    for (int i = 0; i < filesToConvert->size(); ++i){
        // Extraction des données depuis le nom de fichier
        pd.setValue(i);
        QString currentFile = filesToConvert->at(i);
        m_lastPatient->date = currentFile.section('_',0,0);
        m_lastPatient->name = currentFile.section('_',1,1);
        m_lastPatient->surname = currentFile.section('_',2,2);
        convertPDF(QString(m_backupDir + currentFile));
    }
    delete filesToConvert;
}



bool CGestion::renameFile(CPatient* patient, QFile* file, bool _tableUsed)
{
    QString strFileName = constructFileName(patient);
    QString backupFileName = strFileName;

    // Ajout des répertoires:
    strFileName.prepend(m_transferDir);   // Ex: toTransfer/NomFichier.jpg
    backupFileName.prepend(m_backupDir);  // Ex: backup/NomFichier.jpg

    // Vérification du nom de fichier dans le cas où il
    // existerait déjà (erreur d'encodage, reprise...). Modifie le nom sinon.
    while (QFile::exists(strFileName) || QFile::exists(backupFileName)) {
        patient->incrementPage();
        strFileName.clear();
        strFileName = constructFileName(patient);
        backupFileName = strFileName;
        strFileName.prepend(m_transferDir);
        backupFileName.prepend(m_backupDir);
    }

    // Copie du fichier de backup et suppression de l'original.
    if ( !(file.copy(backupFileName)) ||
         !(file.remove()) ) {
        return false;
    }
}



//    // Ouverture du fichier
//    if (m_fileList->size()){
//        QString strFileToOpen;
//        getFile(strFileToOpen);
//        if (strFileToOpen.isNull())
//            return false;

//        QFile file(strFileToOpen);
//        // Création du nouveau nom
//        QString strFileName;
//        QString backupFileName;
//        constructFileName(strFileName);
//        if (strFileName.isNull() || strFileName.isEmpty())
//            return false;

//        backupFileName = strFileName;
//        strFileName.prepend(m_transferDir);
//        backupFileName.prepend(m_backupDir);
//        // Vérification si le nom de fichier n'existe pas déjà (erreur d'encodage, reprise...). Modifie le nom sinon
//        while (QFile::exists(strFileName) || QFile::exists(backupFileName)){
//            m_lastPatient->page++;
//            strFileName.clear();
//            constructFileName(strFileName);
//            backupFileName = strFileName;
//            strFileName.prepend(m_transferDir);     // Ex: toTransfer/NomFichier.jpg
//            backupFileName.prepend(m_backupDir);    // Ex: backup/NomFichier.jpg
//        }
//        // Rem : file.copy(strFileName) : copie du fichier toTransfer, pour le moment mis en commentaire car la synchro se fait avec SyncToy
//        if (!(/*file.copy(strFileName) &&*/ file.copy(backupFileName) && file.remove())){
//            return false;
//        }
//        convertPDF(backupFileName);
//        m_sendremaining++;
//        // Ecriture du nom du patient dans une liste de patient
//        QString fullName = m_lastPatient->name;
//        fullName.append(", ");
//        fullName.append(m_lastPatient->surname);
//        if (!_tableUsed){ // Vérification si on ne s'est pas servi de la table
//            //assert(m_fastsearch);
//            if (!m_fastsearch->isInList(fullName)){ // Vérifie si le patient n'est pas déjà dans la liste, l'ajoute sinon
//                ofstream filePatientList("patientlist.txt", ios_base::out | ios_base::app);
//                if (filePatientList){
//                    filePatientList << m_lastPatient->name.toLatin1().data() << ";" << m_lastPatient->surname.toLatin1().data() << ";" << endl;
//                    filePatientList.close();
//                }
//            }
//        }
//        // Ajout dans la liste des derniers ajoutés
//        //assert(m_lastAdded);
//        fullName.append(QString(" (%1 - %2)").arg(m_lastPatient->page).arg(m_lastPatient->date));
//        m_lastAdded->prepend(fullName);
//        // Ecriture du nom du fichier dans une table, fichier,...
//        /*ofstream fileTransfert("transfertlist.txt", ios_base::out | ios_base::app);
//        if (fileTransfert){
//            fileTransfert << m_lastPatient->date.toLatin1().data() << ";" << m_lastPatient->name.toLatin1().data() << ";" << m_lastPatient->surname.toLatin1().data() <<";" << m_lastPatient->page << endl;
//            fileTransfert.close();
//        }*/
//        return true;
//    }
//    else
//        return false;
}

void CGestion::initCopyFile(){
    m_fileToCopyList = new QStringList;
    QDir dir(m_transferDir);
    QStringList filters;
    filters << "*.pdf" << "*.PDF";
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        m_fileToCopyList->append(fileInfo.fileName());
    }
    m_sendremaining = m_fileToCopyList->size();
    m_ic = 0;
}

bool CGestion::copyNextFile(){
    if (!m_fileToCopyList)
        initCopyFile();
    if (m_ic >= 0 && m_ic < m_fileToCopyList->size()){
        QString strFileToOpen = m_transferDir + m_fileToCopyList->at(m_ic);
        QFile file(strFileToOpen);
        QString strDestinationFile = m_destinationDir + m_fileToCopyList->at(m_ic);
        if (!(file.copy(strDestinationFile) && file.remove())){
            return false;
        }
        m_ic++;
        return true;
    }
    else
        return false;
}

void CGestion::prepareNext()
{
    QString str;
    this->getNextFile(str);
    emit setFile(str);
 //   refreshRemaining();
}

/*
void CGestion::onBtnSearchClicked()
{
    search();
    QString str;
    getFile(str);
    emit setFile(str);
    refreshRemaining();
}

void CGestion::onDeleteFile()
{
    QString strFileToOpen;
    getFile(strFileToOpen);
    if (strFileToOpen.isNull())
        emit errorOccur();
    else {
    QFile file(strFileToOpen);
    if (file.remove())
        this->prepareNext();
    else
        emit errorOccur();
    }
}
*/

#include "CFileManager.h"

#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <QVariant>

#include <QProgressDialog>
#include <QApplication>

#include "CError.h"

// Lecture, écriture de fichier
// A remplacer avec les fonctions Qt ?
#include <fstream>
#include <string>
using namespace std;



CFileManager::CFileManager()
{}

CFileManager::~CFileManager(){
    delete m_fileList;
    delete m_fileToCopyList;
}



void CFileManager::loadConfigFile()
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

bool CFileManager::checkDir()
{
    if ( m_searchDir.isEmpty() ||
         m_transferDir.isEmpty() ||
         m_destinationDir.isEmpty() ||
         m_backupDir.isEmpty() ||
         m_PDFDir.isEmpty()) {
        return false;
    }

    QDir dirSearch(m_searchDir);
    QDir dirTransfer(m_transferDir);
    QDir dirDestination(m_destinationDir);
    QDir dirBackup(m_backupDir);
    QDir dirPDF(m_PDFDir);
    if ( !dirSearch.exists() ||
         !dirTransfer.exists() ||
         !dirDestination.exists() ||
         !dirBackup.exists() ||
         !dirPDF.exists()) {
        return false;
    }

    return true;
}
void CFileManager::onCommandReceived(QString command)
{
    if (command.compare("search") == 0) {
        search();
        refreshRemaining();

        // Envoi de la prochaine image.
        QString fileToLoad = getFile();
        qDebug() << "Loading file :" << fileToLoad;
        QImage image = QImage(fileToLoad);
        emit sendInfo("image", QVariant::fromValue<QImage>(image));
    }
}

void CFileManager::search()
{
    if (checkDir() == false) {
        emit errorOccur(CError::UNKNOWNDIR);
        return;
    }
    // Recherche des fichiers JPG à traiter.
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
    qDebug() << "Fichier JPG trouvé dans " << dir.absolutePath() << ": " << m_fileList->size();

    // Recherche des fichiers PDF à envoyer.
    dir = QDir(m_transferDir);
    QStringList pdfFilters;
    pdfFilters << "*.pdf" << "*.PDF";
    dir.setNameFilters(pdfFilters);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    m_sendremaining = dir.entryInfoList().size();
    m_i = 0;
}



void  CFileManager::convertPDFall()
{
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
    QProgressDialog pd ("Conversion en cours...", "Annuler", 0, filesToConvert->size(), qApp->activeWindow(), Qt::Dialog);
    pd.setAutoClose(false);
    for (int i = 0; i < filesToConvert->size(); ++i){
        // Extraction des données depuis le nom de fichier
        pd.setValue(i);
        QString currentFile = filesToConvert->at(i);
        CPatient::instance()->configure("patient_date", currentFile.section('_',0,0));
        CPatient::instance()->configure("patient_name", currentFile.section('_',1,1));
        CPatient::instance()->configure("patient_surname", currentFile.section('_',2,2));
        convertPDF(QString(m_backupDir + currentFile));
    }
    delete filesToConvert;
}

bool CFileManager::convertPDF(QString _backupFileName){
    // Création du nouveau nom
    QString strFileNamePDF = constructFileName(TYPE_PDF);
    QString strTransferNamePDF;

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

bool CFileManager::renameFile(QFile* file, bool _tableUsed)
{
    QString strFileName = constructFileName(TYPE_JPG);
    QString backupFileName = strFileName;

    // Ajout des répertoires:
    strFileName.prepend(m_transferDir);   // Ex: toTransfer/NomFichier.jpg
    backupFileName.prepend(m_backupDir);  // Ex: backup/NomFichier.jpg

    // Vérification du nom de fichier dans le cas où il
    // existerait déjà (erreur d'encodage, reprise...). Modifie le nom sinon.
    while (QFile::exists(strFileName) || QFile::exists(backupFileName)) {
        CPatient::instance()->incrementPage();
        strFileName.clear();
        strFileName = constructFileName(TYPE_JPG);
        backupFileName = strFileName;
        strFileName.prepend(m_transferDir);
        backupFileName.prepend(m_backupDir);
    }

    // Copie du fichier de backup et suppression de l'original.
    if ( !(file->copy(backupFileName)) ||
         !(file->remove()) ) {
        return false;
    }
    return true;
}

QString CFileManager::constructFileName(int type)
{
    QString fileName = QString();
    CPatient* patient = CPatient::instance();
    switch(type){
    case TYPE_JPG:
        fileName = patient->getParameter("patient_date").toString();
        fileName.append("_");
        fileName.append(patient->getParameter("patient_name").toString());
        fileName.append("_");
        fileName.append(patient->getParameter("patient_surname").toString());
        fileName.append("_");
        fileName.append(patient->getParameter("patient_page").toString());
        fileName.append(".jpg");
        break;
    case TYPE_PDF:
        fileName = patient->getParameter("patient_name").toString();
        fileName.append("_");
        fileName.append(patient->getParameter("patient_surname").toString().toUpper());
        fileName.append("_$_");
        // Conversion de la date au format JJMMAAAA
        QString day, month, year;

        // FIXME
        //extractDate(patient->getParameter("date").toString(), day, month, year);

        fileName.append(day);
        fileName.append(month);
        fileName.append(year);
        fileName.append("_RESUMECONTACT.pdf");
    }
    return fileName;
}


void CFileManager::refreshRemaining()
{
    int iRemaining = (m_fileList->size() - m_i >= 0)? m_fileList->size() : 0;
    int iSendRemaining = m_sendremaining;
    emit sendInfo("remainingImage", QVariant(iRemaining));
    emit sendInfo("remainingPDF", QVariant(iSendRemaining));
}


QString CFileManager::getNextFile(){
    m_i++;
    return getFile();
}

QString CFileManager::getFile(){
    QString resp;
    if (!m_fileList->isEmpty()){
        if (m_i >= 0 && m_i < m_fileList->size())
            resp = m_searchDir + "/" + m_fileList->at(m_i);
    }
    return resp;
}

void CFileManager::setInfo(QString _name, QString _surname, QString _date, int _page){
  /*
    m_lastPatient->name = _name;
    m_lastPatient->surname = _surname;
    m_lastPatient->date = _date;
    m_lastPatient->page = _page;
    */
}

void CFileManager::getInfo(QString& _name, QString& _surname, QString& _date, int& _page){
   /*
    m_lastPatient->page++;
    _name = m_lastPatient->name;
    _surname = m_lastPatient->surname;
    _date = m_lastPatient->date;
    _page = m_lastPatient->page;
  */
}

void CFileManager::setFastSearch(CFastSearch* _fs){
   m_fastsearch = _fs;
}

void CFileManager::setLastAdded(QStringList* _la){
    m_lastAdded = _la;
}

void CFileManager::getDate(QString& _date){
    //_date = m_lastPatient->date;
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
//}

void CFileManager::initCopyFile(){
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

bool CFileManager::copyNextFile(){
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

void CFileManager::prepareNext()
{
//    QString str;
//    this->getNextFile();
//    refreshRemaining();
}

/*
void CFileManager::onBtnSearchClicked()
{
    search();
    QString str;
    getFile(str);
    emit setFile(str);
    refreshRemaining();
}

void CFileManager::onDeleteFile()
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

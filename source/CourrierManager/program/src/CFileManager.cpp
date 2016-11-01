#include "CFileManager.h"

#include <QtPrintSupport/QPrinter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QProgressDialog>

#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <QVariant>
#include <QPainter>
#include <QSettings>

#include "Action/CActionManager.h"
#include "Action/CCancelAction.h"

#include "CError.h"
#include "CDate.h"

using namespace std;

CFileManager::CFileManager()
{
    m_fileList = new QStringList();
    m_fileToCopyList = 0;
    m_sendremaining = 0;
    loadSettings();
}

CFileManager::~CFileManager(){
    delete m_fileList;
    delete m_fileToCopyList;
}


void CFileManager::loadSettings()
{
    QSettings settings;
    if ( settings.status() != QSettings::NoError ||
         !settings.contains("searchDir")   ||
         !settings.contains("transferDir") ||
         !settings.contains("destinationDir") ||
         !settings.contains("backupDir") ||
         !settings.contains("PDFDir") )
    {
        emit errorOccur(CError::NOCONFIGFILE);
        return;
    }

    m_searchDir = settings.value("searchDir").toString();
    m_transferDir = settings.value("transferDir").toString();
    m_destinationDir = settings.value("destinationDir").toString();
    m_backupDir = settings.value("backupDir").toString();
    m_PDFDir = settings.value("PDFDir").toString();

    m_searchDir.append('/');
    m_transferDir.append('/');
    m_destinationDir.append('/');
    m_backupDir.append('/');
    m_PDFDir.append('/');
}


bool CFileManager::checkDir()
{
    if ( m_searchDir.isEmpty() ||
         m_transferDir.isEmpty() ||
         m_backupDir.isEmpty() ||
         m_PDFDir.isEmpty()) {
        return false;
    }

    QDir dirSearch(m_searchDir);
    QDir dirTransfer(m_transferDir);
    QDir dirBackup(m_backupDir);
    QDir dirPDF(m_PDFDir);
    if ( !dirSearch.exists() ||
         !dirTransfer.exists() ||
         !dirBackup.exists() ||
         !dirPDF.exists()) {
        return false;
    }

    return true;
}

void CFileManager::onCommandReceived(QString command)
{
    qDebug() << "FM: command received: " << command;
    if (command.compare("search") == 0) {
        search();
        refreshRemaining();
        // Envoi de la prochaine image.
        int iRemaining = (m_fileList->size() - m_i >= 0)? m_fileList->size() - m_i: 0;
        if (iRemaining > 0) {
            m_currentFileName = getFile();
            QImage image = QImage(m_currentFileName);
            emit sendInfo("image", QVariant::fromValue<QImage>(image));
        }
    }

    // Valider
    if (command.compare("validate") == 0) {
        // Renommer le fichier et ajout potentiel � la table
        if (renameFile()) {
            this->start();
            emit sendInfo("addToHistory", true);
            prepareNext();
            refreshRemaining();
        }
        else{
            emit errorOccur(CError::RENAME);
        }
    }
    // Refresh
    if (command.compare("refresh") == 0) {
        refreshRemaining();
    }

    // Skip
    if (command.compare("skip") == 0) {
        prepareNext();
        refreshRemaining();
    }

    // Delete
    if (command.compare("delete") == 0) {
        deleteFile();
        refreshRemaining();
    }

    // Rotate
    if ( command.compare("rotate") == 0 ) {
        rotateImage();
    }

    // Landscape
    if ( command.compare("landscape") == 0 ) {
        flipLandscape();
    }

}

void CFileManager::run()
{
    QString pdfName = constructFileName(TYPE_PDF);
    QString radicalName = constructFileName(TYPE_RADICAL);
    convertPDF(pdfName, radicalName);
}

void CFileManager::search()
{
    if (checkDir() == false) {
        emit errorOccur(CError::UNKNOWNDIR);
        return;
    }

    // Recherche des fichiers JPG a traiter.
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

    // Recherche des fichiers PDF a envoyer.
    dir = QDir(m_transferDir);
    QStringList pdfFilters;
    pdfFilters << "*.pdf" << "*.PDF";
    dir.setNameFilters(pdfFilters);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    m_sendremaining = dir.entryInfoList().size();
    m_i = 0;
}

QStringList CFileManager::getFilesToSend()
{
    QDir dir(m_transferDir);
    QStringList pdfFilters;
    pdfFilters << "*.pdf" << "*.PDF";
    dir.setNameFilters(pdfFilters);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    const int listSize = list.size();
    QStringList sendList;
    for (int i = 0; i < listSize; ++i) {
        QFileInfo fileInfo = list[i];
        sendList << QString("%1/%2").arg(fileInfo.path()).arg(fileInfo.fileName());
    }
    return sendList;
}



void  CFileManager::convertPDFall()
{
//    // R�cup�ration de la liste des fichiers � copier
//    QDir dir(m_backupDir);
//    QStringList filters;
//    QStringList* filesToConvert = new QStringList;
//    filters << "*.jpg" << "*.JPG";
//    dir.setNameFilters(filters);
//    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
//    QFileInfoList list = dir.entryInfoList();
//    for (int i = 0; i < list.size(); ++i) {
//        QFileInfo fileInfo = list.at(i);
//        filesToConvert->append(fileInfo.fileName());
//    }
//    QProgressDialog pd ("Conversion en cours...", "Annuler", 0, filesToConvert->size(), qApp->activeWindow(), Qt::Dialog);
//    pd.setAutoClose(false);
//    for (int i = 0; i < filesToConvert->size(); ++i){
//        // Extraction des donn�es depuis le nom de fichier
//        pd.setValue(i);
//        QString currentFile = filesToConvert->at(i);
//        CPatient::instance()->configure("patient_date", currentFile.section('_',0,0));
//        CPatient::instance()->configure("patient_name", currentFile.section('_',1,1));
//        CPatient::instance()->configure("patient_surname", currentFile.section('_',2,2));
//        convertPDF(QString(m_backupDir + currentFile));
//    }
//    delete filesToConvert;
}

bool CFileManager::convertPDF(const QString& pdfName, const QString& radicalName){
    // Creation du nouveau nom
    QString strFileNamePDF = pdfName;
    QString strTransferNamePDF;

    strTransferNamePDF = m_transferDir + strFileNamePDF;
    strFileNamePDF.prepend(m_PDFDir);
    // Suppression de l'ancien fichier PDF s'il existe
    if (QFile::exists( strFileNamePDF)){
        QFile formerPDF(strFileNamePDF);
        QFile formerTransferPDF(strTransferNamePDF);
        formerPDF.remove();
        formerTransferPDF.remove();
    } else {
        ++m_sendremaining;
    }
    // Recherche de tous les noms de fichier.
    QStringList listFiles = searchJPGForPatient(radicalName);
    if (listFiles.count() == 0) {
        return false;
    }

    QImage imageToPrint(m_backupDir + listFiles[0]);
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setColorMode(QPrinter::Color);
    printer.setOutputFileName(strFileNamePDF);
    printer.setResolution((int)(imageToPrint.dotsPerMeterX() * 0.0254 + 0.5)); // Dots per meter -> DPI
    printer.setPageMargins(0,0,0,0, QPrinter::Millimeter);
    printer.setPaperSize(QSize(imageToPrint.widthMM(),imageToPrint.heightMM()), QPrinter::Millimeter);
    printer.setOrientation(QPrinter::Portrait);

    QPainter painter(&printer);

    for (int i = 0; i < listFiles.count(); ++i) {
        imageToPrint.load(m_backupDir + listFiles[i]);
        if (!imageToPrint.isNull()) {
            if (i != 0) {
                printer.newPage();
            }
            painter.drawImage(QPoint(0, 0), imageToPrint);
        }
    }
    painter.end();

    // Copie du fichier
    QFile file(strFileNamePDF);
    file.copy(strTransferNamePDF);

    return true;
}


bool CFileManager::renameFile()
{

    QString strFileName = constructFileName(TYPE_JPG);
    QString backupFileName = strFileName;
    QString formerFileName = getFile();
    if (formerFileName.isNull()) {
        return false;
    }
    QFile file(formerFileName);
    // Ajout des repertoires:
    strFileName.prepend(m_transferDir);   // Ex: toTransfer/NomFichier.jpg
    backupFileName.prepend(m_backupDir);  // Ex: backup/NomFichier.jpg


    // Verification du nom de fichier dans le cas ou il
    // existerait deja (erreur d'encodage, reprise...). Modifie le nom sinon.
    while (QFile::exists(strFileName) || QFile::exists(backupFileName)) {
        CPatient::instance()->incrementPage();
        strFileName.clear();
        strFileName = constructFileName(TYPE_JPG);
        backupFileName = strFileName;
        strFileName.prepend(m_transferDir);
        backupFileName.prepend(m_backupDir);
    }

    // Copie du fichier de backup et suppression de l'original.
    if ( !(file.copy(backupFileName)) ||
         !(file.remove()) ) {
        return false;
    }

    // Undo
    CCancelAction* action = new CCancelAction();

    QString strFileNamePDF = constructFileName(TYPE_PDF);
    QString strTransferNamePDF;
    strTransferNamePDF = m_transferDir + strFileNamePDF;
    strFileNamePDF.prepend(m_PDFDir);

    action->setPreviousState(formerFileName);
    action->setCurrentState(backupFileName);
    action->setPdfName(strTransferNamePDF, 0);
    action->setPdfName(strFileNamePDF, 1);
    CActionManager::instance()->push(action);

    return true;
}

QString CFileManager::constructFileName(int type)
{
    QString fileName = QString();
    CPatient* patient = CPatient::instance();
    QString date, day, month, year;

    switch(type){
    case TYPE_JPG:
        fileName = patient->getParameter("patient_date").toString();
        fileName.append("_");
        fileName.append(patient->getParameter("patient_name").toString().toUpper());
        fileName.append("_");
        fileName.append(patient->getParameter("patient_surname").toString().toLower());
        fileName.append("_");
        fileName.append(patient->getParameter("patient_page").toString());
        fileName.append(".jpg");
        break;
    case TYPE_PDF:
        fileName = patient->getParameter("patient_name").toString().toUpper();
        fileName.append("_");
        fileName.append(patient->getParameter("patient_surname").toString().toUpper());
        fileName.append("_$_");
        // Conversion de la date au format JJMMAAAA
        date = patient->getParameter("patient_date").toString();
        CDate::extractDate(date, day, month, year);
        fileName.append(day);
        fileName.append(month);
        fileName.append(year);
        fileName.append("_RESUMECONTACT.pdf");
        break;
    case TYPE_RADICAL:
        fileName = patient->getParameter("patient_date").toString();
        fileName.append("_");
        fileName.append(patient->getParameter("patient_name").toString());
        fileName.append("_");
        fileName.append(patient->getParameter("patient_surname").toString());
        fileName.append("_*");
        break;
    }
    return fileName;
}


void CFileManager::refreshRemaining()
{
    int iRemaining = (m_fileList->size() - m_i >= 0)? m_fileList->size() - m_i: 0;
    m_sendremaining = getFilesToSend().count();
    emit sendInfo("remainingImage", QVariant(iRemaining));
    emit sendInfo("remainingPDF", QVariant(m_sendremaining));
}


QString CFileManager::getNextFile(){
    ++m_i;
    return getFile();
}

QString CFileManager::getFile(){
    QString resp;
    if (!m_fileList->isEmpty()){
        if (m_i >= 0 && m_i < m_fileList->size())
            resp = m_searchDir + m_fileList->at(m_i);
    }
    return resp;
}

void CFileManager::setFastSearch(CFastSearch* _fs){
   m_fastsearch = _fs;
}

void CFileManager::setLastAdded(QStringList* _la){
    m_lastAdded = _la;
}


//    // Ouverture du fichier
//    if (m_fileList->size()){
//        QString strFileToOpen;
//        getFile(strFileToOpen);
//        if (strFileToOpen.isNull())
//            return false;

//        QFile file(strFileToOpen);
//        // Cr�ation du nouveau nom
//        QString strFileName;
//        QString backupFileName;
//        constructFileName(strFileName);
//        if (strFileName.isNull() || strFileName.isEmpty())
//            return false;

//        backupFileName = strFileName;
//        strFileName.prepend(m_transferDir);
//        backupFileName.prepend(m_backupDir);
//        // V�rification si le nom de fichier n'existe pas d�j� (erreur d'encodage, reprise...). Modifie le nom sinon
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
//        if (!_tableUsed){ // V�rification si on ne s'est pas servi de la table
//            //assert(m_fastsearch);
//            if (!m_fastsearch->isInList(fullName)){ // V�rifie si le patient n'est pas d�j� dans la liste, l'ajoute sinon
//                ofstream filePatientList("patientlist.txt", ios_base::out | ios_base::app);
//                if (filePatientList){
//                    filePatientList << m_lastPatient->name.toLatin1().data() << ";" << m_lastPatient->surname.toLatin1().data() << ";" << endl;
//                    filePatientList.close();
//                }
//            }
//        }
//        // Ajout dans la liste des derniers ajout�s
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
    m_currentFileName = getNextFile();
    if ( !m_currentFileName.isEmpty() ) {
        QImage image = QImage(m_currentFileName);
        emit sendInfo("image", QVariant::fromValue<QImage>(image));
    } /*else {
        emit errorOccur(CError::NOMOREFILE);
    }*/
}

void CFileManager::rotateImage()
{
    if ( !m_currentFileName.isEmpty() ){
        QPixmap pixmap = QPixmap(m_currentFileName);
        QMatrix rm;
        rm.rotate(180);
        pixmap = pixmap.transformed(rm);
        pixmap.save( m_currentFileName );
        emit sendInfo("image", QVariant::fromValue<QImage>( pixmap.toImage() ) );
    }
}

void  CFileManager::flipLandscape()
{
    if ( !m_currentFileName.isEmpty() ){
        QPixmap pixmap = QPixmap(m_currentFileName);
        QMatrix rm;
        rm.rotate(90);
        pixmap = pixmap.transformed(rm);
        pixmap.save( m_currentFileName );
        emit sendInfo("image", QVariant::fromValue<QImage>( pixmap.toImage() ) );
    }
}



QStringList CFileManager::searchJPGForPatient(const QString& radicalName)
{
    QStringList list;
    QDir dir (m_backupDir);
    list = dir.entryList(QStringList(radicalName), QDir::Files | QDir::NoSymLinks);
    list.sort();
    return list;
}

bool CFileManager::deleteFile()
{
    QString strFileToOpen = getFile();
    QFile file(strFileToOpen);
    if (file.remove()) {
        prepareNext();
        return true;
    } else {
        emit errorOccur(CError::DELETEFILE);
        return false;
    }
}

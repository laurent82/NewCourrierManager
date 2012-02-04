#ifndef CGESTION_H
#define CGESTION_H

#include <QStringList>
#include <QString>

#include "CFastSearch.h"

#define TYPE_JPG 1
#define TYPE_PDF 2

class PatientInfo{
public:
    PatientInfo(){ name = surname = date = ""; page = 0;}
    QString name;
    QString surname;
    QString date;
    int page;
};

class CGestion : public QObject
{
    Q_OBJECT
public:
    CGestion();
    ~CGestion();
    void loadConfigFile();
    void search();
    void setInfo(QString, QString, QString, int);
    void getInfo(QString&, QString&, QString&, int&);
    void getDate(QString&);
    void extractDate(QString, QString&, QString&, QString&);
    void getRemaining(int&, int&);
    bool renameFile(bool tableUsed = false);
     void initCopyFile();
    bool copyNextFile();
    bool deleteFile();
    void getFile(QString&);
    void getNextFile(QString&);
    void setFastSearch(CFastSearch*);
    void setLastAdded(QStringList*);
    void convertPDFall();

public slots:
    void onBtnSearchClicked();
signals:
    void setFile(QString str);
    void errorOccur(int errorId);
private:

    void constructFileName(QString& f, int type = TYPE_JPG);
    bool convertPDF(QString);
    QStringList* m_fileList;
    QStringList* m_fileToCopyList;
    QStringList* m_lastAdded;   // Liste des derniers courriers traités
    QString m_searchDir;        // Répertoire où se trouve le courrier à classer
    QString m_transferDir;      // Répertoire où le courrier est stocké avant d'etre transféré
    QString m_destinationDir;   // Répertoire où le courrier est envoyé
    QString m_backupDir;        // Répertoire où le courrier est conservé en local
    QString m_PDFDir;           // Répertoire où le courrier est conservé en local sous format PDF
    int m_i;    // Indice de parcours dans le fichier
    int m_ic;   // Indice de parcours des fichiers à copier
    int m_sendremaining;
    PatientInfo* m_lastPatient;
    CFastSearch* m_fastsearch;

};

#endif // CGESTION_H

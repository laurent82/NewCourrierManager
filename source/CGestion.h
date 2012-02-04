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
    QStringList* m_lastAdded;   // Liste des derniers courriers trait�s
    QString m_searchDir;        // R�pertoire o� se trouve le courrier � classer
    QString m_transferDir;      // R�pertoire o� le courrier est stock� avant d'etre transf�r�
    QString m_destinationDir;   // R�pertoire o� le courrier est envoy�
    QString m_backupDir;        // R�pertoire o� le courrier est conserv� en local
    QString m_PDFDir;           // R�pertoire o� le courrier est conserv� en local sous format PDF
    int m_i;    // Indice de parcours dans le fichier
    int m_ic;   // Indice de parcours des fichiers � copier
    int m_sendremaining;
    PatientInfo* m_lastPatient;
    CFastSearch* m_fastsearch;

};

#endif // CGESTION_H

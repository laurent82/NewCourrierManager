#ifndef CGESTION_H
#define CGESTION_H

#include <QStringList>
#include <QString>

#include "CFastSearch.h"
#include "CPatient.h"

#define TYPE_JPG 1
#define TYPE_PDF 2

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
    void onSearch();

    void onValidate(CPatient *patient);
 //   void onDeleteFile();

signals:
    void setFile(QString str);
    void errorOccur(int errorId);
    void sentRefreshRemaining(int, int);
private:
    void prepareNext();

    /**
     * @brief Construit le nom de fichier en fonction du type d�sir�.
     * @param patient Info sur le patient
     * @param type Type de fichier (jpg ou pdf)
     * @return Nom du fichier
     */
    QString constructFileName(CPatient *patient, int type = TYPE_JPG);

    /**
     * @brief Extrait la date du QString d'entr�e.
     * @param date (Entr�e) Date au format YYYYMMDD
     * @param day (Sortie) Jour
     * @param month (Sortie) Mois
     * @param year (Sortie) Ann�e
     */
    void extractDate(const QString& date, QString& day, QString& month, QString& year);


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
    CFastSearch* m_fastsearch;

};

#endif // CGESTION_H

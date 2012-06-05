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
     * @brief Construit le nom de fichier en fonction du type désiré.
     * @param patient Info sur le patient
     * @param type Type de fichier (jpg ou pdf)
     * @return Nom du fichier
     */
    QString constructFileName(CPatient *patient, int type = TYPE_JPG);

    /**
     * @brief Extrait la date du QString d'entrée.
     * @param date (Entrée) Date au format YYYYMMDD
     * @param day (Sortie) Jour
     * @param month (Sortie) Mois
     * @param year (Sortie) Année
     */
    void extractDate(const QString& date, QString& day, QString& month, QString& year);


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
    CFastSearch* m_fastsearch;

};

#endif // CGESTION_H

#ifndef CFILEMANAGER_H
#define CFILEMANAGER_H

#include <QStringList>
#include <QString>
#include <QFile>

#include "CFastSearch.h"
#include "CPatient.h"

class CFileManager : public QObject
{
    Q_OBJECT
public:
    enum{
        TYPE_JPG,
        TYPE_PDF
    };

    CFileManager();
    ~CFileManager();

    ///////////////////////////////////////
    // Configuration
    ///////////////////////////////////////

    void loadConfigFile();

    ///////////////////////////////////////
    // Gestion courrier entrant
    // - recherche des nouveaux fichiers
    // - prochaine image � analyser
    // - suppression d'une image existante.
    ///////////////////////////////////////
    /**
      * Recherche des fichiers JPG, courrier restant � classer (local)
      */
    void search();

    void getNextFile(QString&);

    void getRemaining(int&, int&);

    bool deleteFile();

    ///////////////////////////////////////
    // Gestion du courrier sortant
    // - renommage de fichiers
    // - copie sur le serveur
    ///////////////////////////////////////

    bool renameFile(QFile* file, bool tableUsed = false);

    void initCopyFile();

    bool copyNextFile();

    ///////////////////////////////////////
    // Gestion de masse
    // - Conversion vers un format
    ///////////////////////////////////////

    void convertPDFall();

    ///////////////////////////////////////
    // Autres
    ///////////////////////////////////////

    void setInfo(QString, QString, QString, int);
    void getInfo(QString&, QString&, QString&, int&);
    void getDate(QString&);

    void getFile(QString&);
    void setFastSearch(CFastSearch*);
    void setLastAdded(QStringList*);

public slots:
    void onSearch();

    void onValidate();
 //   void onDeleteFile();

signals:
    void setFile(QString str);
    void errorOccur(int errorId);
    void sentRefreshRemaining(int, int);
private:

    void prepareNext();

    /**
     * @brief Extrait la date du QString d'entr�e.
     * @param date (Entr�e) Date au format YYYYMMDD
     * @param day (Sortie) Jour
     * @param month (Sortie) Mois
     * @param year (Sortie) Ann�e
     */
    void extractDate(const QString& date, QString& day, QString& month, QString& year);

    bool convertPDF(QString);

    /**
      * Cr�e le nom du fichier en fonction du type
      * @param type Type (TYPE_JPG, TYPE_PDF)
      */
    QString constructFileName(int type);


    QStringList* m_fileList;
    QStringList* m_fileToCopyList;
    QStringList* m_lastAdded;   /// Liste des derniers courriers trait�s
    QString m_searchDir;        /// R�pertoire o� se trouve le courrier � classer (en JPG)
    QString m_transferDir;      /// R�pertoire o� le courrier est stock� avant d'etre transf�r�
    QString m_destinationDir;   /// R�pertoire o� le courrier est envoy�
    QString m_backupDir;        /// R�pertoire o� le courrier est conserv� en local
    QString m_PDFDir;           /// R�pertoire o� le courrier est conserv� en local sous format PDF

    int m_i;    // Indice de parcours dans le fichier
    int m_ic;   // Indice de parcours des fichiers � copier
    int m_sendremaining;
    CFastSearch* m_fastsearch;

};

#endif // CFileManager_H

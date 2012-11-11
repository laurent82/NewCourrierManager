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
    // - prochaine image à analyser
    // - suppression d'une image existante.
    ///////////////////////////////////////
    /**
      * Recherche des fichiers JPG, courrier restant à classer (local)
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
     * @brief Extrait la date du QString d'entrée.
     * @param date (Entrée) Date au format YYYYMMDD
     * @param day (Sortie) Jour
     * @param month (Sortie) Mois
     * @param year (Sortie) Année
     */
    void extractDate(const QString& date, QString& day, QString& month, QString& year);

    bool convertPDF(QString);

    /**
      * Crée le nom du fichier en fonction du type
      * @param type Type (TYPE_JPG, TYPE_PDF)
      */
    QString constructFileName(int type);


    QStringList* m_fileList;
    QStringList* m_fileToCopyList;
    QStringList* m_lastAdded;   /// Liste des derniers courriers traités
    QString m_searchDir;        /// Répertoire où se trouve le courrier à classer (en JPG)
    QString m_transferDir;      /// Répertoire où le courrier est stocké avant d'etre transféré
    QString m_destinationDir;   /// Répertoire où le courrier est envoyé
    QString m_backupDir;        /// Répertoire où le courrier est conservé en local
    QString m_PDFDir;           /// Répertoire où le courrier est conservé en local sous format PDF

    int m_i;    // Indice de parcours dans le fichier
    int m_ic;   // Indice de parcours des fichiers à copier
    int m_sendremaining;
    CFastSearch* m_fastsearch;

};

#endif // CFileManager_H

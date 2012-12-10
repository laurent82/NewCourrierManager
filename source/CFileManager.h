#ifndef CFILEMANAGER_H
#define CFILEMANAGER_H

#include <QStringList>
#include <QString>
#include <QFile>
#include <QThread>

#include "CFastSearch.h"
#include "CPatient.h"

class CFileManager : public QThread
{
    Q_OBJECT
public:
    enum{
        TYPE_JPG,
        TYPE_PDF,
        TYPE_RADICAL
    };

    CFileManager();
    ~CFileManager();

    void run(); // Thread function

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

    void getRemaining(int&, int&);

    bool deleteFile();

    ///////////////////////////////////////
    // Gestion du courrier sortant
    // - renommage de fichiers
    // - pr�paration de la liste des fichiers � copier
    ///////////////////////////////////////

    bool renameFile(/*QFile* file, bool tableUsed = false*/);

    QStringList getFilesToSend();

    void initCopyFile(); // Obsol�te

    bool copyNextFile(); // Obsol�te

    ///////////////////////////////////////
    // Gestion de masse
    // - Conversion vers un format
    ///////////////////////////////////////

    void convertPDFall();

    ///////////////////////////////////////
    // Autres
    ///////////////////////////////////////

    void setFastSearch(CFastSearch*);
    void setLastAdded(QStringList*);

public slots:
    void onCommandReceived(QString command);

 //   void onDeleteFile();

signals:
    void errorOccur(int errorId);
    void sendInfo(QString key,  QVariant value);

private:
    bool checkDir();

    QString getFile();

    QString getNextFile();

    void refreshRemaining();

    void prepareNext();

    bool convertPDF(const QString &pdfName, const QString &radicalName);

    /**
      * Recherche tous les fichiers JPG correspondant
      * � un patient pour la date du fichier en cours.
      */
    QStringList searchJPGForPatient(const QString &radicalName);

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

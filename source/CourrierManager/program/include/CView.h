#ifndef COURRIERMANAGER_H
#define COURRIERMANAGER_H

#define USE_QT5 1

#ifdef USE_QT5
#include <QtWidgets/QDialog>
#include <QtWidgets/QProgressDialog>
#include <QtWidgets/QFormLayout>
#else
#include <QDialog>
#include <QProgressDialog>
#endif

#include <QResizeEvent>

#include "CDrawPanelWidget.h"
#include "CFastSearch.h"
#include <QStringListModel>
/**
  * Interface graphique
  *
  */

namespace Ui
{
    class CourrierManagerClass;
}

class CView : public QDialog
{
    Q_OBJECT
public:
    CView(QWidget *parent = 0);
    ~CView();

    QStringList* getPatientList() ;

    void setProgressBar(int total);
    void updateProgressBar(int step = 1);
    void updateNetworkMethod();

protected:
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject* obj, QEvent* event);

public slots:
    void onInfoReceived(QString key, QVariant value);
    void displayError(int errorId);
    void onConnectedToHost();
    void onDisconnectedFromHost();
    void onActionUndone(int);
    void onOCRListChanged( QString );
protected slots:
    // Bouton d'actions
    void onButtonClicked(); // Slot activ� par un bouton, destin� � envoyer un message au controller.

    // Histoire
    void onHistoryCancel();

    // Boutons pour les dates et raccourcis.
    void on_btnToday_clicked();
    void on_btnLastDate_clicked();
    void on_btnSamePatient_clicked();

    // Slots utilis�s pour la gestion de la recherche rapide.
    void on_txtName_textEdited(QString);
    void on_txtSurname_textEdited(QString);
    void on_tablePatient_clicked();

    void onTableRefresh();

signals:
    void sendCommand(QString);
    void btnConfigurationClicked();

//    void btnValidateClicked();
//    void criticalError();
//    void deleteFile();

private:
    /**
      * Rempli les infos de CPatient avec les diff�rents champs.
      */
    void fillPatient();

    /**
      * Vide tous les champs
      */
    void clearFields();

    /**
      * @brief Compl�te les champs "date" avec la date du jour.
      */
    void setCurrentDate();

    /**
      * @brief Compl�te les champs "date" avec une date sp�cifique
      */
    void setCustomDate( const QString& day, const QString& month, const QString& year);


    /**
      * @brief Construit un string repr�sentant la date du jour.
      * @return  La date est format YYYYMMDD
      */
    QString constructDate();

    /**
      * @brief V�rifie que tous les champs sont remplis.
      */
    bool checkFields();

    void removeFromHistory (int id);

    bool validate();

    void selectFirstFastSearch();

    QString m_lastDate; /// Derni�re date enregistr�e.

    QStringListModel *m_modele;
    CFastSearch* m_fastsearch;
    CDrawPanelWidget* m_panel;
    QProgressDialog* m_progress;
    bool m_tableUsed;

    Ui::CourrierManagerClass *ui;
};

#endif // COURRIERMANAGER_H

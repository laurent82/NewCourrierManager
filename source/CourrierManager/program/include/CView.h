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
    void setProgressBar(int total);
    void updateProgressBar(int step = 1);
    void updateNetworkMethod();

protected:
    void resizeEvent(QResizeEvent *event);

public slots:
    void onInfoReceived(QString key, QVariant value);
    void displayError(int errorId);
    void onConnectedToHost();
    void onDisconnectedFromHost();
    void onActionUndone(int);

protected slots:
    // Bouton d'actions
    void onButtonClicked(); // Slot activé par un bouton, destiné à envoyer un message au controller.

    // Histoire
    void onHistoryCancel();

    // Boutons pour les dates et raccourcis.
    void on_btnToday_clicked();
    void on_btnLastDate_clicked();
    void on_btnSamePatient_clicked();

    // Slots utilisés pour la gestion de la recherche rapide.
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
      * @brief Vide les champs et remets la date du jour.
      * Ne touche pas aux informations dans CPatient.
      */
    void resetInfoPatient();

    /**
      * Rempli les infos de CPatient avec les différents champs.
      */
    void fillPatient();

    /**
      * Vide tous les champs
      */
    void clearFields();

    /**
      * @brief Complète les champs "date" avec la date du jour.
      */
    void setCurrentDate();

    /**
      * @brief Construit un string représentant la date du jour.
      * @return  La date est format YYYYMMDD
      */
    QString constructDate();

    /**
      * @brief Vérifie que tous les champs sont remplis.
      */
    bool checkFields();

    void removeFromHistory (int id);

    QString m_lastDate; /// Dernière date enregistrée.

    QStringListModel *m_modele;
    CFastSearch* m_fastsearch;
    Ui::CourrierManagerClass *ui;
    CDrawPanelWidget* m_panel;
    QProgressDialog* m_progress;
    bool m_tableUsed;
    bool m_Copying;


};

#endif // COURRIERMANAGER_H

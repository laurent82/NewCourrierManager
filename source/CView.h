#ifndef COURRIERMANAGER_H
#define COURRIERMANAGER_H

#include <QtGui/QDialog>
#include <QResizeEvent>

#include "CDrawPanelWidget.h"
#include "CFastSearch.h"
#include <QStringListModel>
#include <QProgressDialog>
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

protected:
    void resizeEvent(QResizeEvent *event);

public slots:
    void onInfoReceived(QString key, QVariant value);
    void displayError(int errorId);
    void onConnectedToHost();
    void onDisconnectedFromHost();

protected slots:
    // Bouton d'actions
    void onButtonClicked(); // Slot activ� par un bouton, destin� � envoyer un message au controller.

    // Boutons pour les dates et raccourcis.
    void on_btnToday_clicked();
    void on_btnLastDate_clicked();
    void on_btnSamePatient_clicked();

    // Slots utilis�s pour la gestion de la recherche rapide.
    void on_txtName_textEdited(QString);
    void on_txtSurname_textEdited(QString);
    void on_tablePatient_clicked();

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
      * @brief Construit un string repr�sentant la date du jour.
      * @return  La date est format YYYYMMDD
      */
    QString constructDate();

    /**
      * @brief V�rifie que tous les champs sont remplis.
      */
    bool checkFields();

    QString m_lastDate; /// Derni�re date enregistr�e.

    QStringListModel *m_modele;
    QStringList* m_history;
    QStringListModel* m_historyModel;
    CFastSearch* m_fastsearch;
    Ui::CourrierManagerClass *ui;
    CDrawPanelWidget* m_panel;
    QProgressDialog* m_progress;
    bool m_tableUsed;
    bool m_Copying;


};

#endif // COURRIERMANAGER_H

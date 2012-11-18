#ifndef COURRIERMANAGER_H
#define COURRIERMANAGER_H

#include <QtGui/QDialog>
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

protected:
    void resizeEvent(QResizeEvent *event);

public slots:
    void onInfoReceived(QString key, QVariant value);
    void displayError(int errorId);

//    void onPrepareNext(const QString&);
protected slots:
    // Bouton d'actions
    void onButtonClicked(); // Slot activé par un bouton, destiné à envoyer un message au controller.

    // Boutons pour les dates et raccourcis.
    void on_btnToday_clicked();
    void on_btnLastDate_clicked();
    void on_btnSamePatient_clicked();

    // Slots utilisés pour la gestion de la recherche rapide.
    void on_txtName_textEdited(QString);
    void on_txtSurname_textEdited(QString);
    void on_tablePatient_clicked();

 //   void on_btnValidate_clicked(); // Signal émis lorsque les infos de la page sont validés.


//    void on_btnSend_clicked();
//    void on_btnNext_clicked();
//    void on_btnDelete_clicked();
//    void on_btnConvert_clicked();
//    void copyCancel();

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
     * @brief Extrait la date du QString d'entrée.
     * @param date (Entrée) Date au format YYYYMMDD
     * @param day (Sortie) Jour
     * @param month (Sortie) Mois
     * @param year (Sortie) Année
     */
    void extractDate(const QString& date, QString& day, QString& month, QString& year);

    /**
      * @brief Vérifie que tous les champs sont remplis.
      */
    bool checkFields();

    QString m_lastDate; /// Dernière date enregistrée.

    QStringListModel *m_modele;
    QStringList* m_history;
    QStringListModel* m_historyModel;
    CFastSearch* m_fastsearch;
    Ui::CourrierManagerClass *ui;
    CDrawPanelWidget* m_panel;
    bool m_tableUsed;
    bool m_Copying;


};

#endif // COURRIERMANAGER_H

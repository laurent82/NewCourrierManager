#ifndef COURRIERMANAGER_H
#define COURRIERMANAGER_H

#include <QtGui/QDialog>
#include <QResizeEvent>

#include "CGestion.h"
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

private:
    void refreshRemaining();
    void prepareNext();
    void resetInfoPatient();
    void setCurrentDate();
    bool constructDate(QString &_date);
    QStringListModel *m_modele;
    QStringList* m_lastAdded;
    QStringListModel* m_lastAddedModel;
 //   CGestion *m_gestion;
    CFastSearch* m_fastsearch;
    Ui::CourrierManagerClass *ui;
    CDrawPanelWidget* m_panel;
    bool m_tableUsed;
    bool m_Copying;

protected:
    void resizeEvent(QResizeEvent *event);

public slots:
    void onSetFile(QString);
    void displayError(int errorId);
protected slots:
    void on_btnSearch_clicked();
    void on_btnValidate_clicked();
    void on_btnSamePatient_clicked();
    void on_btnToday_clicked();
    void on_btnLastDate_clicked();
    void on_txtName_textEdited(QString);
    void on_txtSurname_textEdited(QString);
    void on_tablePatient_clicked();
    void on_btnSend_clicked();
    void on_btnNext_clicked();
    void on_btnDelete_clicked();
    void on_btnConvert_clicked();
    void copyCancel();

signals:
    void btnSearchClicked();
    void btnValidateClicked();
    void criticalError();
};

#endif // COURRIERMANAGER_H

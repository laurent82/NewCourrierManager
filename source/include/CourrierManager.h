#ifndef COURRIERMANAGER_H
#define COURRIERMANAGER_H

#include <QtGui/QDialog>
#include <QResizeEvent>

#include "CGestion.h"
#include "CDrawPanel.h"
#include "CFastSearch.h"
#include <QStringListModel>
namespace Ui
{
    class CourrierManagerClass;
}

class CourrierManager : public QDialog
{
    Q_OBJECT
public:
    CourrierManager(QWidget *parent = 0);
    ~CourrierManager();

private:
    void refreshRemaining();
    void prepareNext();
    void resetInfoPatient();
    void setCurrentDate();
    bool constructDate(QString &_date);
    QStringListModel *m_modele;
    QStringList* m_lastAdded;
    QStringListModel* m_lastAddedModel;
    CGestion *gestion;
    CFastSearch* m_fastsearch;
    Ui::CourrierManagerClass *ui;
    CDrawPanel* panel;
    bool m_tableUsed;
    bool m_Copying;
    //    QPushButton

protected:
    void resizeEvent(QResizeEvent *event);


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
};

#endif // COURRIERMANAGER_H

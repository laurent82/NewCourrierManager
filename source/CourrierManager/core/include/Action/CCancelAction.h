#ifndef CCANCELACTION_H
#define CCANCELACTION_H

#include "Action/CAction.h"
#include <QString>


class CCancelAction : public CAction
{
    Q_OBJECT
public:
    CCancelAction();
    QString getActionType() const;
    bool undo();

    void setPdfName(const QString& pdfName, int id);
    QString getPdfName(int id) const;

    void setSingleCancel(bool single);

signals:
    void undoDone(QString);

private:
    QString m_pdfName[2];
    bool m_single;
};

#endif

#include "Action/CCancelAction.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>

CCancelAction::CCancelAction() :
    CAction()
{
    m_single = false;
}

QString CCancelAction::getActionType() const
{
    return QString("cancel");
}

bool CCancelAction::undo()
{
    // JPG
    QFile currentFile(m_currentState.toString());
    if (currentFile.exists() == false)
    {
        qDebug() << "Current file does NOT exist";
        return false;
    }
    bool copyOk = currentFile.copy(m_previousState.toString());

    // PDF
    // Note les pdf peuvent deja avoir ete supprimee par une autre action.
    QFile filePdf(m_pdfName[0]);
    if (filePdf.exists())
    {
        filePdf.remove();
    }
    QFile filePdf1(m_pdfName[1]);
    if (filePdf1.exists())
    {
        filePdf1.remove();
    }

    bool final = copyOk && currentFile.remove();

    if (final && m_single == false) {
        emit undoDone(m_pdfName[0]);
    }
    return final;
}

void CCancelAction::setPdfName(const QString &pdfName, int id)
{
    m_pdfName[id] = pdfName;
}

QString CCancelAction::getPdfName(int id) const
{
    return m_pdfName[id];
}

void CCancelAction::setSingleCancel(bool single)
{
    m_single = single;
}



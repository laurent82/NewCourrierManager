#ifndef CPATIENT_H
#define CPATIENT_H

#include <QVariant>
#include <QString>
#include <QMap>

class CPatient
{
public:
    CPatient();
    void configure(const QString& key, const QVariant& value);
    void clear();
    QVariant getParameter(const QString& key) const;

    /**
     * @brief Incrémenter le numéro de page de 1.
     */
    void incrementPage();

private:
   QVariantMap m_parameters;
};

#endif // CPATIENT_H

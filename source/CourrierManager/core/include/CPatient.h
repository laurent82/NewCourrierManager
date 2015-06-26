#ifndef CPATIENT_H
#define CPATIENT_H

#include <QVariant>
#include <QString>
#include <QMap>

class CPatient
{
public:
    static CPatient* instance();

    /**
      * Parameteres disponibles:
      * patient_name :     Nom du patient       (QString)
      * patient_surname:   Prénom du patient    (QString)
      * patient_date:      Date du document     (QString)
      * patient_page:      Numero de la page    (int)
      * patient_table:     Le patient provient de la table (bool)
      */
    QVariant getParameter(const QString& key) const;

    void configure(const QString& key, const QVariant& value);

    /**
      *  Efface toutes les informations de l'instance.
      */
    void clear();

    /**
     * @brief Incremente le numero de page de 1.
     */
    void incrementPage();

protected:
    CPatient();
private:
   QVariantMap m_parameters;
   static CPatient* m_instance;
};

#endif // CPATIENT_H

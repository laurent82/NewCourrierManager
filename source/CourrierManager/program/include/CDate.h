#ifndef CDATE_H
#define CDATE_H

#include <QString>
#include <QDate>

class CDate
{
public:
    CDate();

    /**
     * @brief Extrait la date du QString d'entr�e.
     * @param date (Entr�e) Date au format YYYYMMDD
     * @param day (Sortie) Jour
     * @param month (Sortie) Mois
     * @param year (Sortie) Ann�e
     */
    static void extractDate(const QString& date, QString& day, QString& month, QString& year);
};

#endif // CDATE_H

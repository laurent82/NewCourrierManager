#ifndef CDATE_H
#define CDATE_H

#include <QString>
#include <QDate>

class CDate
{
public:
    CDate();

    /**
     * @brief Extrait la date du QString d'entrée.
     * @param date (Entrée) Date au format YYYYMMDD
     * @param day (Sortie) Jour
     * @param month (Sortie) Mois
     * @param year (Sortie) Année
     */
    static void extractDate(const QString& date, QString& day, QString& month, QString& year);
};

#endif // CDATE_H

#include "CDate.h"

CDate::CDate()
{
}

void  CDate::extractDate(const QString& date, QString& day, QString& month, QString& year)
{
    QChar c;
    QString var = "";

    // Récupère l'année
    for (int i = 0; i < 4; ++i){
        c = date.at(i);
        var.append(c);
    }
    year = var;
    // Récupère le mois
    var.clear();
    c = date.at(4);
    var.append(c);
    c = date.at(5);
    var.append(c);
    month = var;
    // Récupère le jour
    var.clear();
    c = date.at(6);
    var.append(c);
    c = date.at(7);
    var.append(c);
    day = var;
}


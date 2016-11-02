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

void CDate::extractDateFromOcr(const QString& date, QString& day, QString& month, QString& year)
{
    QChar seperator;
    if ( date.contains("/") )
    {
        seperator = '/';
    }
    else
    {
        seperator = '-';
    }
    QStringList date_list = date.split( seperator );
    day = date_list.at(0);
    month = date_list.at(1);
    year = date_list.at(2);
}

bool CDate::isDate(const QString& date)
{
    QRegExp reDate("[0-9]{1,2}[/-][0-9]{2,2}[/-][0-9]{2,4}");
    return reDate.exactMatch( date ); 
}


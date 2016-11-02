#ifndef CFASTSEARCH_H
#define CFASTSEARCH_H

#include <QThread>
#include <QStringList>
#include <QVector>


class CFastSearch : public QThread
{
public:
    CFastSearch(bool _bDirectSearch = true);
    ~CFastSearch();
    void run();
    void setWord(QString);
    void getItem(int, QString&) const ;
    void getCurrentItem(int, QString&) const ;
    bool isInList(QString) const;
    QStringList* getCurrentList() const;
    QStringList* getFullList();
    void appendNewPatientInFile();

private:
    void readPatientList();
    int computeScore(int) const;
    QStringList* m_patientList;
    QStringList* m_currentList;
    QString m_word;
    int m_size;
    bool m_searching;
    bool m_fullSorted;

};

#endif // CFASTSEARCH_H

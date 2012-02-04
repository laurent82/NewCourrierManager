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
    void getItem(int, QString&);
    bool isInList(QString);
    QStringList* getCurrentList();
    QStringList* getFullList();
private:
    void readPatientList();
    int computeScore(int);
    QStringList* m_patientList;
    QStringList* m_currentList;
    QString m_word;
    int m_size;
    bool m_searching;
    bool m_fullSorted;
 //   QVector<int> m_score;
};

#endif // CFASTSEARCH_H

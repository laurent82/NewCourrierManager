#include "CFastSearch.h"

#include <string>
#include <fstream>
using namespace std;

CFastSearch::CFastSearch(bool _bDirectSearch)
{
    m_patientList = new QStringList;
    m_currentList = new QStringList;
    m_searching = false;
    if (_bDirectSearch){
        readPatientList();
        m_fullSorted = true;
    }
    else
        m_fullSorted = false;
}

CFastSearch::~CFastSearch(){
    delete m_patientList;
    delete m_currentList;
}


void CFastSearch::readPatientList(){
    ifstream file("./patientlist.txt");
    if (file){
        string line;
        while (!file.eof()){
            getline(file, line);
            QString strline(line.c_str());
            if (!strline.isNull() && !strline.isEmpty()){
                QString strToInsert = strline.section(';',0,0);
                strToInsert.append(", ");
                strToInsert.append(strline.section(';', 1, 1));
                m_patientList->append(strToInsert);
            }
        }
    }
    m_patientList->sort();
}

void CFastSearch::setWord(QString _word){
    m_word = _word;
    m_size = m_word.size();
}

int CFastSearch::computeScore(int i){
    int iScore = 0;
    const QString *currentWord = &m_patientList->at(i);
    int iMin = (m_size < currentWord->size())? m_size : currentWord->size();

    for (int i = 0; i < iMin; ++i){
        if (m_word.at(i) == currentWord->at(i)){
            iScore++;
        }
        else
            return iScore;
    }
    return iScore;
}

QStringList* CFastSearch::getCurrentList(){
    return m_currentList;
}

QStringList* CFastSearch::getFullList(){
    if (!m_fullSorted){
        m_patientList->sort();
        m_fullSorted = true;
    }
    m_currentList->clear();
    return m_patientList;
}

void CFastSearch::getItem(int i, QString& _rep){
    if (m_currentList->size() > 0){
        if (i >= 0 && i < m_currentList->size()){
            _rep = QString(m_currentList->at(i));
        }
    }
    else{
            if (i >= 0 && i < m_patientList->size()){
            _rep = QString(m_patientList->at(i));
        }
    }
//    qWarning("%s", _rep.toLatin1().data());
}

bool CFastSearch::isInList(QString _word){
    if (m_patientList->contains(_word,Qt::CaseInsensitive))
        return true;
    else
    {
        m_patientList->append(_word);
        m_fullSorted = false;
        return false;
    }
}

void CFastSearch::run(){
    m_currentList->clear();
    int iBestScore = 1;
    for (int i = 0; i < m_patientList->size(); ++i){
        int iRep = computeScore(i);
        if (iRep > iBestScore){
            m_currentList->clear();
            iBestScore = iRep;
            m_currentList->append(m_patientList->at(i));
        }
        else if (iRep == iBestScore){
            m_currentList->append(m_patientList->at(i));
        }
    }
    m_currentList->sort();
}



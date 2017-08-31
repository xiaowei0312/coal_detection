#ifndef DATA_DEPENDENCY_H
#define DATA_DEPENDENCY_H

#include <QObject>
#include <QLineEdit>
#include <QString>
#include <QList>

class DataDependency : public QObject
{
private: 
    double __execFormula(char *szFormula);
public:
    QLineEdit *pTarget;
    QList<QLineEdit *> depList;
    QString formula;
public:
    DataDependency();
    bool depHasEmptyElement();
    void markEmptyDepElement();
    double execFormula();
};

#endif // DATA_DEPENDENCY_H

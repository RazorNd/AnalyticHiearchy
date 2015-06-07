#ifndef ALTERNATIVESMATRIX_H
#define ALTERNATIVESMATRIX_H

#include "abstractprioritizationmatrix.h"
#include <QString>

class AlternativesMatrix : public AbstractPrioritizationMatrix
{
    Q_OBJECT
    int _numberCriteria;
public:
    AlternativesMatrix(int numberCriteria, AnalyticHiearchyModel * parentModel);

    QString criteriaName() const;

signals:
    void criteriaNameChanged(QString name);

public slots:
    void setCriteriaName(QString name);    

    // AbstractPrioritizationMatrix interface
protected:
    QVariant ratingHeaderData(int section, int role) const;
};

#endif // ALTERNATIVESMATRIX_H

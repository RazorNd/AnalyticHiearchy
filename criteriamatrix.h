#ifndef CRITERIAMATRIX_H
#define CRITERIAMATRIX_H

#include "abstractprioritizationmatrix.h"
#include <QVariant>

class CriteriaMatrix : public AbstractPrioritizationMatrix
{
    Q_OBJECT
public:
    CriteriaMatrix(AnalyticHiearchyModel * parentModel);

    // AbstractPrioritizationMatrix interface
protected:
    QVariant ratingHeaderData(int section, int role) const;
};

#endif // CRITERIAMATRIX_H

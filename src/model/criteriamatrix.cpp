#include "criteriamatrix.h"

CriteriaMatrix::CriteriaMatrix(AnalyticHiearchyModel *parentModel):
    AbstractPrioritizationMatrix(parentModel)
{
    setSize(parentModel->criteriaCount());
    connect(parentModel, SIGNAL(criteriaCountChanged(int)), SLOT(setSize(int)));
}

QVariant CriteriaMatrix::ratingHeaderData(int section, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return parent()->criterion(section);
    }
    return QVariant();
}


QString CriteriaMatrix::getName() const
{
    return "Критерии";
}

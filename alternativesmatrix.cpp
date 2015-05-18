#include "alternativesmatrix.h"

AlternativesMatrix::AlternativesMatrix(int numberCriteria, AnalyticHiearchyModel *parentModel):
    AbstractPrioritizationMatrix(parentModel),
    _numberCriteria(numberCriteria)
{
    setSize(parentModel->alternativeCount());
    connect(parentModel, SIGNAL(alternativeCountChanged(int)), SLOT(setSize(int)));    
}

QString AlternativesMatrix::criteriaName() const
{
    return parent()->criterion(_numberCriteria);
}

void AlternativesMatrix::setCriteriaName(QString name)
{
    emit criteriaNameChanged(name);
}

QVariant AlternativesMatrix::ratingHeaderData(int section, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return parent()->alternative(section);
    }
    return QVariant();
}

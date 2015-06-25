#include "src/model/resultmatrix.h"
#include "src/model/alternativesmatrix.h"
#include "src/model/criteriamatrix.h"
#include <QColor>

bool ResultMatrix::isRatingCell(const QModelIndex &index) const
{
    if(!index.isValid())
        return false;
    if(index.column() >= _parent->criteriaCount())
        return false;
    return true;
}

bool ResultMatrix::isResultCell(const QModelIndex &index) const
{
    if(!index.isValid())
        return false;
    if(index.column() == _parent->criteriaCount())
        return true;
    return false;
}

double ResultMatrix::priority(int alternative, int criterion) const
{
    return _parent->alternativesMatrix(criterion)->normalPriority(alternative) *
            _parent->criteriaMatrix()->normalPriority(criterion);
}

QVariant ResultMatrix::priorityData(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return QVariant(priority(index.row(), index.column()));
    case Qt::BackgroundColorRole:
        if(isBestPriority(index))
            return QColor(Qt::green);
    }
    return QVariant();
}

double ResultMatrix::globalPriority(int alternative) const
{
    const int size = _parent->criteriaCount();
    double result = 0.0;
    for(int i = 0; i < size; i++)
    {
        result += priority(alternative, i);
    }
    return result;
}

QString ResultMatrix::bestAlternative() const
{
    if(_parent && _bestGlobalPriority < _parent->alternativeCount())
        return _parent->alternative(_bestGlobalPriority);
    return QString();
}

QVariant ResultMatrix::globalPriorityData(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return QVariant(globalPriority(index.row()));
    case Qt::BackgroundColorRole:
        if(isBestGlobalPriority(index))
            return QColor(Qt::green);
    }
    return QVariant();
}

void ResultMatrix::checkBestPriority(int criterion, int alternative, double value)
{
    if(_bestPriority.at(criterion).second < value)
        _bestPriority[criterion] = qMakePair(alternative, value);
}

bool ResultMatrix::isBestPriority(int criterion, int alternative) const
{
    if(criterion >= _bestPriority.size())
        return false;
    return _bestPriority.at(criterion).first == alternative;
}

bool ResultMatrix::isBestPriority(const QModelIndex &index) const
{
    if(index.isValid())
        return isBestPriority(index.column(), index.row());
    return false;
}

bool ResultMatrix::isBestGlobalPriority(int alternative) const
{
    return alternative == _bestGlobalPriority;
}

bool ResultMatrix::isBestGlobalPriority(const QModelIndex &index) const
{
    if(index.isValid())
        return isBestGlobalPriority(index.row());
    return false;
}

void ResultMatrix::calculateBestPriority()
{
    const int maxCriterion = _parent->criteriaCount();
    const int maxAlternatives = _parent->alternativeCount();
    for(int criterion = 0; criterion < maxCriterion; criterion++)
    {
        for(int alternative = 0; alternative < maxAlternatives; alternative++)
        {
            checkBestPriority(criterion, alternative,
                              priority(alternative, criterion));
        }
    }
}

void ResultMatrix::calculateBestGlobalPriority()
{
    const int maxAlternatives = _parent->alternativeCount();
    double max = 0.0;
    for(int i = 0; i < maxAlternatives; i++)
    {
        double priority = globalPriority(i);
        if(priority > max)
        {
            _bestGlobalPriority = i;
            max = priority;
        }
    }
    emit bestAlternativeChanged(_parent->alternative(_bestGlobalPriority));
}

void ResultMatrix::clear()
{
    _bestPriority.clear();
    _bestPriority.resize(_parent->criteriaCount());
    _bestGlobalPriority = -1;
    qFill(_bestPriority, qMakePair(-1, 0.0));
}

ResultMatrix::ResultMatrix(AnalyticHiearchyModel *parentModel):
    QAbstractTableModel(parentModel), _parent(parentModel)
{    
    connect(parentModel, SIGNAL(dataChanged()), SLOT(updateDate()));
    connect(parentModel, SIGNAL(alternativeNameChanged(int,QString)),
            SLOT(onAlterntiveNameChanged(int,QString)));
    clear();
}


int ResultMatrix::rowCount(const QModelIndex &) const
{
    return _parent->alternativeCount();
}

int ResultMatrix::columnCount(const QModelIndex &) const
{
    return _parent->criteriaCount() + 1;
}

QVariant ResultMatrix::data(const QModelIndex &index, int role) const
{
    if(isRatingCell(index))
        return priorityData(index, role);
    if(isResultCell(index))
        return globalPriorityData(index, role);
    return QVariant();    
}

QVariant ResultMatrix::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
        switch (orientation) {
        case Qt::Horizontal:
            if(section == columnCount() - 1)
                return "Глобальные приоритеты";
            return _parent->criterion(section);
        case Qt::Vertical:
            return _parent->alternative(section);
        }
    return QVariant();
}

void ResultMatrix::updateDate()
{
    beginResetModel();
    clear();
    calculateBestPriority();
    calculateBestGlobalPriority();
    endResetModel();
}

void ResultMatrix::onAlterntiveNameChanged(int numberAlternative, QString name)
{
    if(numberAlternative == _bestGlobalPriority)
        emit bestAlternativeChanged(name);
}

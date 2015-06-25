#include "abstractprioritizationmatrix.h"
#include <QColor>
#include <QtMath>
#include <QDebug>


AnalyticHiearchyModel *AbstractPrioritizationMatrix::parent() const
{
    return _parent;
}

bool AbstractPrioritizationMatrix::setHarmonizationCorrect(bool value)
{
    _harmonizationCorrect = value;
    emit harmonizationCorcted(_harmonizationCorrect);
    return _harmonizationCorrect;
}

bool AbstractPrioritizationMatrix::isRatingCell(const QModelIndex &index) const
{
    if(index.row() < sumRatingRow() && index.column() < vectorPriorityColumn()
            && index.isValid())
        return true;
    return false;
}

bool AbstractPrioritizationMatrix::isSumCell(const QModelIndex &index) const
{
    if(index.isValid() && index.row() == sumRatingRow()
            && index.column() < (int)size())
        return true;
    return false;
}

bool AbstractPrioritizationMatrix::isVectorPriorityCell(const QModelIndex &index) const
{
    if(index.isValid() && index.column() == vectorPriorityColumn()
            && index.row() < (int)size())
        return true;
    return false;
}

bool AbstractPrioritizationMatrix::isNormalVectorPriorityCell(const QModelIndex &index) const
{
    if(index.isValid() && index.column() == normalVectorPriorityColumn()
            && index.row() < (int)size())
        return true;
    return false;
}

bool AbstractPrioritizationMatrix::isSumVectorPriorityCell(const QModelIndex &index) const
{
    if(index.isValid() && index.row() == sumRatingRow()
            && index.column() == vectorPriorityColumn())
        return true;
    return false;
}

QVariant AbstractPrioritizationMatrix::ratingData(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        bool correct;
        int rating = SkewSymmetricMatrix::data(index, role).toInt(&correct);
        if(correct)
            return _parent->scale()->ratingToString(rating);
    }
    return SkewSymmetricMatrix::data(index, role);
}

QVariant AbstractPrioritizationMatrix::sumData(const QModelIndex &index, int role) const
{
    double data = _sumRating.at(index.column());
    switch (role) {
    case Qt::DisplayRole:
        if(data)
            return data;
        break;
    case Qt::BackgroundColorRole:
        if(!data)
            return QColor(Qt::red);
        return QColor(Qt::yellow);
    }
    return QVariant();
}

QVariant AbstractPrioritizationMatrix::vectorPriorityData(const QModelIndex &index, int role) const
{
    double data = _vectorPriority.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        if(data)
            return data;
        break;
    case Qt::BackgroundColorRole:
        if(!data)
            return QColor(Qt::red);
        return QColor(Qt::yellow);
    }
    return QVariant();
}

QVariant AbstractPrioritizationMatrix::normalVectorPriorityData(const QModelIndex &index, int role) const
{    
    double data = normalPriority(index.row());
    switch (role) {
    case Qt::DisplayRole:
        if(data)
            return data;
        break;
    case Qt::BackgroundColorRole:
        if(!data)
            return QColor(Qt::red);
        return QColor(Qt::yellow);
    }
    return QVariant();
}

QVariant AbstractPrioritizationMatrix::sumVectorPriorityData(int role) const
{
    double data = _sumVectoPriority;
    switch (role) {
    case Qt::DisplayRole:
        if(data)
            return data;
        break;
    case Qt::BackgroundColorRole:
        if(!data)
            return QColor(Qt::red);
        return QColor(Qt::yellow);
    }
    return QVariant();
}

double AbstractPrioritizationMatrix::normalPriority(int at) const
{
    if(at > _vectorPriority.size())
        return 0.0;
    return _sumVectoPriority ?
                _vectorPriority.at(at) / _sumVectoPriority :
                0.0;
}

bool AbstractPrioritizationMatrix::recalculateSumRating(int pos)
{
    double result = 0;
    for(int i = 0; i < (int)size(); i++)
    {
        QVariant value = index(i, pos).data(Qt::EditRole);
        if(!value.isValid())
            return false;

        int rating = value.toInt();
        result += _parent->scale()->ratingToDouble(rating);
    }
    _sumRating[pos] = result;
    emit dataChanged(index(sumRatingRow(), pos), index(sumRatingRow(), pos));
    return true;
}

bool AbstractPrioritizationMatrix::recalculateVectorPriority(int pos)
{
    double result = 1;
    for(int i = 0; i < (int)size(); i++)
    {
        QVariant value = index(pos, i).data(Qt::EditRole);
        if(!value.isValid())
            return false;

        int rating = value.toInt();
        result *= _parent->scale()->ratingToDouble(rating);
    }
    _vectorPriority[pos] = qPow(result, 1.0 / size());

    emit dataChanged(index(pos, vectorPriorityColumn()),
                     index(pos, normalVectorPriorityColumn()));

    _sumVectoPriority = 0;
    foreach (const double value, _vectorPriority)
    {
        _sumVectoPriority += value;
    }
    emit dataChanged(index(size(), size()), index(size(), size()));
    emit NormalVectorPriorityChanged();
    return true;
}

void AbstractPrioritizationMatrix::recalculateHarmonization()
{
    double sum = 0;
    int size = sumRatingRow();
    for(int i = 0; i < size; i++)
    {
        double sumRating = _sumRating.at(i);
        double priority = normalPriority(i);
        if(!sumRating || !priority)
        {
            setHarmonizationCorrect(false);
            return;
        }
        sum += sumRating * priority;
    }

    double indexHarmonization = (sum - size) / (size - 1);
    _harmonization = indexHarmonization / _parent->randomConsistency(size);

    emitHarmonization();
}

void AbstractPrioritizationMatrix::emitHarmonization()
{
    if(_harmonization >= 0)
    {
        QString color;
        if(_harmonization < 0.1)
            color = "green";
        else if (_harmonization < 0.2)
            color = "yellow";
        else
            color = "red";

        setHarmonizationCorrect(true);
        emit harmonizationChanged(_harmonization);
        emit harmonizationChanged(QString("<b>%1%</b>")
                                  .arg(QString::number(_harmonization * 100, 'f', 2)));
        emit harmonizationColorChanged(QString("background-color: %1")
                                       .arg(color));
    }
    else
    {
        setHarmonizationCorrect(false);
        emit harmonizationChanged("");
        emit harmonizationColorChanged(QString("background-color: white"));
    }
}

int AbstractPrioritizationMatrix::sumRatingRow() const
{
    return (int)size();
}

int AbstractPrioritizationMatrix::vectorPriorityColumn() const
{
    return (int)size();
}

int AbstractPrioritizationMatrix::normalVectorPriorityColumn() const
{
    return vectorPriorityColumn() + 1;
}

AbstractPrioritizationMatrix::AbstractPrioritizationMatrix(AnalyticHiearchyModel *parentModel):
    SkewSymmetricMatrix(parentModel),
    _parent(parentModel),
    _sumVectoPriority(0.0),
    _harmonization(0.0),
    _harmonizationCorrect(false)
{
    connect(this, SIGNAL(dataChanged(QModelIndex,QModelIndex)), parentModel, SIGNAL(dataChanged()));
    connect(parentModel, SIGNAL(maxRatingChanged(int)), SLOT(clear()));
}

double AbstractPrioritizationMatrix::harmonization() const
{
    if(_harmonizationCorrect)
        return _harmonization;
    return 0;
}

const RatingScale *AbstractPrioritizationMatrix::scale() const
{
    return parent()->scale();
}

int AbstractPrioritizationMatrix::rowCount(const QModelIndex &) const
{
    return size() + 1;
}

int AbstractPrioritizationMatrix::columnCount(const QModelIndex &) const
{
    return size() + 2;
}

QVariant AbstractPrioritizationMatrix::data(const QModelIndex &index, int role) const
{
    if(isRatingCell(index))
        return ratingData(index, role);
    if(isSumCell(index))
        return sumData(index, role);
    if(isVectorPriorityCell(index))
        return vectorPriorityData(index, role);
    if(isNormalVectorPriorityCell(index))
        return normalVectorPriorityData(index, role);
    if(isSumVectorPriorityCell(index))
        return sumVectorPriorityData(role);

    return QVariant();
}


void AbstractPrioritizationMatrix::setSize(int size)
{
    SkewSymmetricMatrix::setSize(size);
    _sumRating.resize(size);
    _vectorPriority.resize(size);
    qFill(_sumRating, 0);
    qFill(_vectorPriority, 0);
}

void AbstractPrioritizationMatrix::clear()
{
    beginResetModel();
    qFill(_sumRating, 0);
    qFill(_vectorPriority, 0);
    _sumVectoPriority = 0;
    _harmonization = 0.0;
    emitHarmonization();
    SkewSymmetricMatrix::clear();
    endResetModel();
}

QVariant AbstractPrioritizationMatrix::sumRatingHeaderData(int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return "Сумма рейтингов";
    }
    return QVariant();
}

QVariant AbstractPrioritizationMatrix::priorityHeaderData(int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return "Вектор приоритета";
    }
    return QVariant();
}

QVariant AbstractPrioritizationMatrix::normalPriorityHeaderData(int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return "Нормальзованный вектор приоритета";
    }
    return QVariant();
}

bool AbstractPrioritizationMatrix::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(isRatingCell(index) && role == Qt::EditRole)
    {
        bool correct;
        int rating = value.toInt(&correct);
        if(correct && _parent->scale()->ratingIsCorrect(rating))
        {
            SkewSymmetricMatrix::setData(index, value, role);
            if(recalculateSumRating(index.column()) &&
                    recalculateVectorPriority(index.row()) &&
                    recalculateSumRating(index.row()) &&
                    recalculateVectorPriority(index.column()))
            {
                recalculateHarmonization();
            }
            return true;
        }
    }
    return false;
}

QVariant AbstractPrioritizationMatrix::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(section < sumRatingRow())
        return ratingHeaderData(section, role);
    if(orientation == Qt::Vertical && section == sumRatingRow())
        return sumRatingHeaderData(role);
    if(orientation == Qt::Horizontal)
    {
        if(section == vectorPriorityColumn())
            return priorityHeaderData(role);
        if(section == normalVectorPriorityColumn())
            return normalPriorityHeaderData(role);
    }
    return QVariant();
}


Qt::ItemFlags AbstractPrioritizationMatrix::flags(const QModelIndex &index) const
{
    if(isRatingCell(index))
        return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
    if(isVectorPriorityCell(index) || isNormalVectorPriorityCell(index) ||
            isSumCell(index) || isSumVectorPriorityCell(index))
        return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
    return Qt::ItemNeverHasChildren;
}

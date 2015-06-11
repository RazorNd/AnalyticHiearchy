#include "entringdatamodel.h"
#include "criteriamatrix.h"
#include "alternativesmatrix.h"
#include <QDebug>
#include <QVariant>
#include <QtAlgorithms>


EntringDataModel::TypeCell EntringDataModel::getTypeCell(const QModelIndex &index) const
{
    if(!index.isValid())
        return EntringDataModel::IncorrectCell;
    if(index.row() > 1)
        return EntringDataModel::AlternativeRating;
    if(index.row() == 1)
        return EntringDataModel::CriteriaRating;
    return EntringDataModel::CriteriaDirection;
}

bool EntringDataModel::setCriteriaRating(int criteria, const QVariant &value)
{
    if(value.isNull())
    {
        _criteriaRating[criteria] = 0;
        return true;
    }
    bool ok;
    int rating = value.toInt(&ok);
    if(ok)
        _criteriaRating[criteria] = rating;
    return ok;
}

bool EntringDataModel::setCriteriaDirection(int criteria, const QVariant &value)
{
    if(value.isNull())
        return false;
    _criteriaDirectionOnMax.setBit(criteria, value.toBool());
    return true;
}

bool EntringDataModel::setAlternativesRating(int criteria, int alternative, const QVariant &value)
{
    if(value.isNull())
    {
        _alternativeRating[criteria][alternative] = 0;
        return true;
    }
    bool ok;
    double rating = value.toDouble(&ok);
    if(ok)
        _alternativeRating[criteria][alternative] = rating;
    return ok;
}

void EntringDataModel::enterCriteria()
{
    int size = _parent->criteriaCount();
    int max = 0;
    foreach (int val, _criteriaRating)
        if(val > max)
            max = val;

    CriteriaMatrix *matrix = _parent->criteriaMatrix();
    for(int i = 0; i < size; i++)
    {
        for(int j = i + 1; j < size; j++)
        {
            if(_parent->maxRating() != max)
            {
                double delta =
                        (double)(_criteriaRating[i] - _criteriaRating[j])*_parent->maxRating() / max;
                int res = qRound(qAbs(delta) - 1);
                matrix->setData(matrix->index(i, j), delta > 0 ? res : -res, Qt::EditRole);
            }
            else
            {
                int delta =  _criteriaRating[i] - _criteriaRating[j];
                matrix->setData(matrix->index(i, j), delta, Qt::EditRole);
            }
        }
    }
}

void EntringDataModel::enterAlternatives()
{
    int size = _parent->alternativeCount();
    QVector<double> nullVector(size, 0.0);
    for(int i = 0; i < _alternativeRating.size(); i++)
    {
        if(!qEqual(_alternativeRating[i].begin(),
                   _alternativeRating[i].end(),
                   nullVector.begin()))
        {
            enterAlternative(i);
        }
    }
}

void EntringDataModel::enterAlternative(int criteria)
{
    QVector<double> &alternative = _alternativeRating[criteria];
    int size = _parent->alternativeCount();
    AlternativesMatrix *matrix = _parent->alternativesMatrix(criteria);
    double max, min;
    max = min = alternative[0];
    foreach (double val, alternative)
    {
        if(val > max)
            max = val;
        if(val < min)
            min = val;
    }
    for(int i = 0; i < size; i++)
    {
        for(int j = i + 1; j < size; j++)
        {
            double delta =
                    (alternative[i] - alternative[j])
                    * (_parent->maxRating() - 1)
                    / (max - min);
            int res = qRound(delta);
            matrix->setData(matrix->index(i, j),
                            _criteriaDirectionOnMax.at(criteria) ? res : -res,
                            Qt::EditRole);
        }
    }
}

EntringDataModel::EntringDataModel(AnalyticHiearchyModel *parentModel):
    QAbstractTableModel(parentModel), _parent(parentModel)
{
}

void EntringDataModel::setCriteriaCount(int count)
{
    int size = _criteriaRating.size();
    if(count == size)
        return;
    if(count > size)
    {
        beginInsertColumns(QModelIndex(), size, count - 1);
        _criteriaRating.resize(count);
        _criteriaDirectionOnMax.resize(count);
        _alternativeRating.resize(count);
        int alternativeCount = _parent->alternativeCount();
        for(int i = size; i < count; i++)
        {
            _alternativeRating[i].resize(alternativeCount);
        }
        endInsertColumns();
        return;
    }
    beginRemoveColumns(QModelIndex(), count, size - 1);
    _criteriaRating.resize(count);
    _criteriaDirectionOnMax.resize(count);
    _alternativeRating.resize(count);
    endRemoveColumns();
}

void EntringDataModel::setAlternativesCount(int count)
{
    int size;
    if(!_alternativeRating.size())
        return;
    if((size = _alternativeRating[0].size()) == count)
        return;
    else if(count > size)
        beginInsertRows(QModelIndex(), size + 2, count + 1);
    else
        beginRemoveRows(QModelIndex(), count + 2, size + 1);


    for(int i = 0; i < _alternativeRating.size(); i++)
    {
        _alternativeRating[i].resize(count);
    }
    if(count > size)
        endInsertRows();
    else
        endRemoveRows();
}

void EntringDataModel::clear()
{
    beginResetModel();
    _criteriaRating.fill(0);
    _criteriaDirectionOnMax.fill(false);
    for(QVector<QVector<double> >::iterator it = _alternativeRating.begin();
        it != _alternativeRating.end(); it++)
    {
        it->fill(0.0);
    }
    endResetModel();
}

int EntringDataModel::rowCount(const QModelIndex &/*parent*/) const
{
    return _parent->alternativeCount() + 2;
}

int EntringDataModel::columnCount(const QModelIndex &/*parent*/) const
{
    return _parent->criteriaCount();
}

QVariant EntringDataModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        if(getTypeCell(index) == EntringDataModel::CriteriaDirection)
        {
            return _criteriaDirectionOnMax.at(index.column()) ? "max" : "min";
        }
    case Qt::EditRole:
        switch(getTypeCell(index)){
        case EntringDataModel::AlternativeRating:
            return _alternativeRating.at(index.column()).at(index.row() - 2);
        case EntringDataModel::CriteriaRating:
            return _criteriaRating.at(index.column());
        case EntringDataModel::CriteriaDirection:
            return _criteriaDirectionOnMax.at(index.column());
        default:
            return QVariant();
        }
        break;
    default:
        return QVariant();
    }
}

bool EntringDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole)
        return false;

    switch(getTypeCell(index)){
    case EntringDataModel::AlternativeRating:
        return setAlternativesRating(index.column(), index.row() - 2, value);
    case EntringDataModel::CriteriaRating:
        return setCriteriaRating(index.column(), value);
    case EntringDataModel::CriteriaDirection:
        return setCriteriaDirection(index.column(), value);
    default:
        return false;
    }
}

QVariant EntringDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(role)
    {
    case Qt::DisplayRole:
        if(section == 1 && orientation == Qt::Vertical)
            return QString("Важность критерия");
        if(section == 0 && orientation == Qt::Vertical)
            return QString("Экстремум критерия");
    case Qt::EditRole:
        if(orientation == Qt::Horizontal)
            return _parent->criterion(section);
        else if(section > 1)
            return _parent->alternative(section - 2);
    }
    return QVariant();
}

bool EntringDataModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if(role != Qt::EditRole)
        return false;

    if(orientation == Qt::Horizontal)
    {
        QStringListModel *model = _parent->criteriaModel();
        return model->setData(model->index(section), value);
    }
    if(section > 1)
    {
        QStringListModel *model = _parent->alternativesModel();
        return model->setData(model->index(section - 2), value);
    }
    return false;
}

Qt::ItemFlags EntringDataModel::flags(const QModelIndex &/*index*/) const
{
    Qt::ItemFlags flag = Qt::ItemIsSelectable |
            Qt::ItemIsEditable |
            Qt::ItemIsEnabled |
            Qt::ItemNeverHasChildren;    
    return flag;
}


void EntringDataModel::enterModel()
{
    enterCriteria();
    enterAlternatives();
}

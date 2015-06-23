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
    if(index.row() != CriteriaDirection)
        return EntringDataModel::AlternativeRating;    
    return EntringDataModel::CriteriaDirection;
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
    if(!value.isValid())
    {
        _alternativeRating[criteria][alternative] = 0;
        _alternativeRatingDefined[criteria][alternative] = false;
        return true;
    }
    bool ok;
    double rating = value.toDouble(&ok);
    if(ok)
    {
        _alternativeRating[criteria][alternative] = rating;
        _alternativeRatingDefined[criteria][alternative] = true;
    }
    return ok;
}

bool EntringDataModel::setCriteriaDirection(const QModelIndex &index, const QVariant &value)
{
    return setCriteriaDirection(index.column(), value);
}

bool EntringDataModel::setAlternativesRating(const QModelIndex &index, const QVariant &value)
{
    return setAlternativesRating(index.column(), index.row() - 1, value);
}

void EntringDataModel::enterAlternatives()
{    
    for(int i = 0; i < _alternativeRating.size(); i++)
    {        
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
    if(max == min)
        return;
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
    int size = _alternativeRating.size();
    if(count == size)
        return;
    if(count > size)
    {
        beginInsertColumns(QModelIndex(), size, count - 1);        
        _criteriaDirectionOnMax.resize(count);
        _alternativeRating.resize(count);
        _alternativeRatingDefined.resize(count);
        int alternativeCount = _parent->alternativeCount();
        for(int i = size; i < count; i++)
        {
            _alternativeRating[i].resize(alternativeCount);
            _alternativeRatingDefined[i].resize(alternativeCount);
        }
        endInsertColumns();
        return;
    }
    beginRemoveColumns(QModelIndex(), count, size - 1);    
    _criteriaDirectionOnMax.resize(count);
    _alternativeRating.resize(count);
    _alternativeRatingDefined.resize(count);
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
        _alternativeRatingDefined[i].resize(count);
    }
    if(count > size)
        endInsertRows();
    else
        endRemoveRows();
}

void EntringDataModel::clear()
{
    beginResetModel();    
    _criteriaDirectionOnMax.fill(false);
    int critetiaCount = _alternativeRating.size();
    for(int i = 0; i < critetiaCount; i++)
    {        
        _alternativeRating[i].fill(0.0);
        _alternativeRatingDefined[i].fill(false);
    }
    endResetModel();
}

int EntringDataModel::rowCount(const QModelIndex &/*parent*/) const
{
    return _parent->alternativeCount() + 1;
}

int EntringDataModel::columnCount(const QModelIndex &/*parent*/) const
{
    return _parent->criteriaCount();
}

QVariant EntringDataModel::alternativeRating(const QModelIndex &index, int role) const
{

    if(_alternativeRatingDefined.at(index.column()).at(index.row() - 1)
            || role == Qt::EditRole)
    {
        return _alternativeRating.at(index.column()).at(index.row() - 1);
    }
    return QVariant();
}

void EntringDataModel::clear(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    switch (getTypeCell(index)) {
    case EntringDataModel::AlternativeRating:
        setAlternativesRating(index, QVariant());
        break;
    default:
        break;
    }
    emit dataChanged(index, index, QVector<int>() << Qt::EditRole);
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
            return alternativeRating(index, role);        
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
    {
        return false;
    }

    switch(getTypeCell(index)){
    case EntringDataModel::AlternativeRating:
        return setAlternativesRating(index, value);    
    case EntringDataModel::CriteriaDirection:
        return setCriteriaDirection(index, value);
    default:
        return false;
    }
}

QVariant EntringDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(role)
    {
    case Qt::DisplayRole:        
        if(section == 0 && orientation == Qt::Vertical)
            return QString("Экстремум критерия");
    case Qt::EditRole:
        if(orientation == Qt::Horizontal)
            return _parent->criterion(section);
        else if(section > CriteriaDirection)
            return _parent->alternative(section - 1);
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
    enterAlternatives();
}


EntringDataModel::EntringDataSelectionModel::EntringDataSelectionModel(EntringDataModel *model, QObject *parent):
    QItemSelectionModel(model, parent),
    _model(model)
{
}

void EntringDataModel::EntringDataSelectionModel::deleteSelected()
{
    QModelIndexList indexes = selectedIndexes();
    if(!indexes.size())
        return _model->clear();
    foreach (const QModelIndex &index, indexes)
    {
        _model->clear(index);
    }
}

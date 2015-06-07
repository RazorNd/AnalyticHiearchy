#include "skewsymmetricmatrix.h"
#include <QColor>

void SkewSymmetricMatrix::setSkewSymetricData(const QModelIndex &firstIndex, int value)
{
    QModelIndex secondIndex = skewSymmetricIndex(firstIndex);
    _data[firstIndex] = value;
    _data[secondIndex] = -value;
    emit dataChanged(firstIndex, firstIndex);
    emit dataChanged(secondIndex, secondIndex);
}

QModelIndex SkewSymmetricMatrix::skewSymmetricIndex(const QModelIndex &firstIndex) const
{
    return index(firstIndex.column(), firstIndex.row());
}

SkewSymmetricMatrix::SkewSymmetricMatrix(QObject *parent):
    QAbstractTableModel(parent), _size(0)
{
}

int SkewSymmetricMatrix::size() const
{
    return _size;
}

void SkewSymmetricMatrix::setSize(int size)
{
    if(size == _size || size == 0)
        return;
    void (SkewSymmetricMatrix::*callBackColumn)();
    void (SkewSymmetricMatrix::*callBackRow)();
    if(size > _size)
    {
        beginInsertColumns(QModelIndex(), _size, size - 1);
        beginInsertRows(QModelIndex(), _size, size - 1);
        callBackColumn = &endInsertColumns;
        callBackRow = &endInsertRows;
    }
    else
    {
        beginRemoveColumns(QModelIndex(), size, _size - 1);
        beginRemoveRows(QModelIndex(), size, _size - 1);
        callBackColumn = &endRemoveColumns;
        callBackRow = &endRemoveRows;
    }
    _size = size;
    (this->*callBackColumn)();
    (this->*callBackRow)();
}

int SkewSymmetricMatrix::rowCount(const QModelIndex &) const
{
    return _size;
}

int SkewSymmetricMatrix::columnCount(const QModelIndex &) const
{
    return _size;
}

QVariant SkewSymmetricMatrix::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    bool hasData = _data.contains(index);

    switch (role) {
    case Qt::DisplayRole:
        if(index.row() == index.column())
            return 0;
        if(hasData)
            return _data.value(index);
        else
            return QVariant();
        break;
    case Qt::EditRole:
        return _data.value(index, 0);
    case Qt::BackgroundColorRole:
        if(index.column() == index.row())
            return QColor(Qt::green);
        if(!hasData)
            return QColor(Qt::red);
        break;
    }
    return QVariant();
}

bool SkewSymmetricMatrix::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        bool correct;
        int data = value.toInt(&correct);
        if(correct)
        {
            setSkewSymetricData(index, data);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags SkewSymmetricMatrix::flags(const QModelIndex &)const
{    
    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
}

void SkewSymmetricMatrix::clear()
{
    beginResetModel();
    _data.clear();
    endResetModel();
}

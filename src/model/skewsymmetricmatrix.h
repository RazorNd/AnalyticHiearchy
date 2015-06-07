#ifndef SKEWSYMMETRICMATRIX_H
#define SKEWSYMMETRICMATRIX_H

#include <QAbstractItemModel>
#include <QHash>

class SkewSymmetricMatrix : public QAbstractTableModel
{
    Q_OBJECT
    int _size;
    QHash<QModelIndex, int> _data;
    void setSkewSymetricData(const QModelIndex &firstIndex, int value);
    QModelIndex skewSymmetricIndex(const QModelIndex &firstIndex) const;
public:
    SkewSymmetricMatrix(QObject *parent = 0);


    virtual int size() const;

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &) const;
    virtual int columnCount(const QModelIndex &) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &) const;

public slots:
    virtual void clear();
    virtual void setSize(int size);
};

#endif // SKEWSYMMETRICMATRIX_H

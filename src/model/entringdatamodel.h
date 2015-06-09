#ifndef ENTRINGDATAMODEL_H
#define ENTRINGDATAMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QBitArray>
#include "analytichiearchymodel.h"

class EntringDataModel : public QAbstractTableModel
{
    Q_OBJECT

    enum TypeCell {AlternativeRating, CriteriaRating, CriteriaDirection, IncorrectCell};

    AnalyticHiearchyModel * const _parent;

    QVector<int> _criteriaRating;    
    QBitArray _criteriaDirectionOnMax;
    QVector< QVector<double> > _alternativeRating;

    TypeCell getTypeCell(const QModelIndex &index) const;

    bool setCriteriaRating(int criteria, const QVariant &value);
    bool setCriteriaDirection(int criteria, const QVariant &value);
    bool setAlternativesRating(int criteria, int alternative, const QVariant &value);

    void enterCriteria();
    void enterAlternatives();
    void enterAlternative(int criteria);
public:
    EntringDataModel(AnalyticHiearchyModel *parentModel);    

    void setCriteriaCount(int count);
    void setAlternativesCount(int count);

public slots:
    void clear();
    void enterModel();

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // ENTRINGDATAMODEL_H

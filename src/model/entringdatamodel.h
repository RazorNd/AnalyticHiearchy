#ifndef ENTRINGDATAMODEL_H
#define ENTRINGDATAMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QBitArray>
#include <QItemSelectionModel>
#include "analytichiearchymodel.h"

class EntringDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    class EntringDataSelectionModel;

private:

    enum TypeCell {AlternativeRating, CriteriaRating, CriteriaDirection, IncorrectCell};

    AnalyticHiearchyModel * const _parent;

    QVector<uint> _criteriaRating;
    QBitArray _criteriaRatingDefinded;
    QBitArray _criteriaDirectionOnMax;

    QVector< QVector<double> > _alternativeRating;
    QVector<QBitArray> _alternativeRatingDefined;

    TypeCell getTypeCell(const QModelIndex &index) const;

    bool setCriteriaRating(int criteria, const QVariant &value);
    bool setCriteriaDirection(int criteria, const QVariant &value);
    bool setAlternativesRating(int criteria, int alternative, const QVariant &value);
    bool setCriteriaRating(const QModelIndex &index, const QVariant &value);
    bool setCriteriaDirection(const QModelIndex &index, const QVariant &value);
    bool setAlternativesRating(const QModelIndex &index, const QVariant &value);

    void enterCriteria();
    void enterAlternatives();
    void enterAlternative(int criteria);
    QVariant alternativeRating(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant criteriaRating(const QModelIndex &index, int role = Qt::DisplayRole) const;

public:
    EntringDataModel(AnalyticHiearchyModel *parentModel);    

    void setCriteriaCount(int count);
    void setAlternativesCount(int count);

    void clear(const QModelIndex& index);

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

class EntringDataModel::EntringDataSelectionModel
        : public QItemSelectionModel
{
    Q_OBJECT
    EntringDataModel *_model;
public:
    EntringDataSelectionModel(EntringDataModel *model, QObject *parent = 0);

public slots:
    void deleteSelected();
};

#endif // ENTRINGDATAMODEL_H

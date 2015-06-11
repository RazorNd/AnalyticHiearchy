#ifndef ABSTRACTPRIORITIZATIONMATRIX_H
#define ABSTRACTPRIORITIZATIONMATRIX_H

#include "skewsymmetricmatrix.h"
#include "src/rating.h"
#include "analytichiearchymodel.h"
#include <QVector>

class AnalyticHiearchyModel;

class AbstractPrioritizationMatrix : public SkewSymmetricMatrix
{
    Q_OBJECT

    AnalyticHiearchyModel * const _parent;
    QVector<double> _sumRating;
    QVector<double> _vectorPriority;
    double _sumVectoPriority;
    double _harmonization;
    bool _harmonizationCorrect;

    bool setHarmonizationCorrect(bool value);

    QVariant ratingData(const QModelIndex &index, int role) const;
    QVariant sumData(const QModelIndex &index, int role) const;
    QVariant vectorPriorityData(const QModelIndex &index, int role) const;
    QVariant normalVectorPriorityData(const QModelIndex &index, int role) const;
    QVariant sumVectorPriorityData(int role) const;



    bool recalculateSumRating(int pos);
    bool recalculateVectorPriority(int pos);    

    int sumRatingRow() const;
    int vectorPriorityColumn() const;
    int normalVectorPriorityColumn() const;
    void recalculateHarmonization();

    void emitHarmonization();

public:
    bool isRatingCell(const QModelIndex &index) const;
    bool isSumCell(const QModelIndex &index) const;
    bool isVectorPriorityCell(const QModelIndex &index) const;
    bool isNormalVectorPriorityCell(const QModelIndex &index) const;
    bool isSumVectorPriorityCell(const QModelIndex &index) const;

    AbstractPrioritizationMatrix(AnalyticHiearchyModel *parentModel);

    double harmonization() const;
    const RatingScale *scale() const;
    AnalyticHiearchyModel *parent() const;

    double normalPriority(int at) const;

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;    
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    // SkewSymmetricMatrix interface
    void setSize(int size);
    void clear();

    virtual QString getName() const = 0;
signals:
    void harmonizationChanged(QString value);
    void harmonizationChanged(double value);
    void harmonizationColorChanged(QString styleSheet);
    void harmonizationCorcted(bool value);
    void NormalVectorPriorityChanged();
protected:
    virtual QVariant ratingHeaderData(int section, int role) const = 0;
    QVariant sumRatingHeaderData(int role) const;
    QVariant priorityHeaderData(int role) const;
    QVariant normalPriorityHeaderData(int role) const;
};

#endif // ABSTRACTPRIORITIZATIONMATRIX_H

#ifndef RESULTMATRIX_H
#define RESULTMATRIX_H

#include "analytichiearchymodel.h"
#include <QAbstractTableModel>
#include <QObject>
#include <QVector>
#include <QPair>

class ResultMatrix : public QAbstractTableModel
{
    Q_OBJECT
    AnalyticHiearchyModel * const _parent;
    QVector<QPair<int, double> > _bestPriority;
    int _bestGlobalPriority;

    bool isRatingCell(const QModelIndex& index) const;
    bool isResultCell(const QModelIndex& index) const;

    double priority(int alternative, int criterion) const;
    QVariant priorityData(const QModelIndex& index, int role) const;

    double globalPriority(int alternative) const;
    QVariant globalPriorityData(const QModelIndex& index, int role) const;

    void checkBestPriority(int criterion, int alternative, double value);
    bool isBestPriority(int criterion, int alternative) const;
    bool isBestPriority(const QModelIndex& index) const;
    bool isBestGlobalPriority(int alternative) const;
    bool isBestGlobalPriority(const QModelIndex& index) const;

    void calculateBestPriority();
    void calculateBestGlobalPriority();

    void clear();
public:
    ResultMatrix(AnalyticHiearchyModel *parentModel);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:
    void bestAlternativeChanged(QString);

private slots:
    void updateDate();
    void onAlterntiveNameChanged(int numberAlternative, QString name);
};

#endif // RESULTMATRIX_H

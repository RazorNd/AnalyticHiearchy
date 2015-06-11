#ifndef ANALYTICHIEARCHYMODEL_H
#define ANALYTICHIEARCHYMODEL_H

#include "src/rating.h"
#include <QStringList>
#include <QStringListModel>
#include <QVector>

class CriteriaMatrix;
class AlternativesMatrix;
class ResultMatrix;
class EntringDataModel;

class AnalyticHiearchyModel : public QObject
{
    Q_OBJECT

    static double _randomConsistency[11];

    RatingScale _scale;
    QStringListModel _criteria;
    QStringListModel _alternatives;
    int _criteriaCount;
    int _alternativeCount;

    CriteriaMatrix *_criteriaMatrix;
    QVector<AlternativesMatrix *> _alternativesMatrixs;
    ResultMatrix *_result;
    EntringDataModel *_entringData;

    void createAlternativesMatrixs();
    void updateAlternativesMatrixs();
    void deleteAlternativesMatrixs();

    void updateCriteria();
    void updateAlternatives();

    void appendCriterias(int count);
    void appendAlternatives(int count);

    void removeLastCriterias(int count);
    void removeLastAlternatives(int count);
public:
    AnalyticHiearchyModel(QObject *parent = 0);

    static double randomConsistency(int size);
    const RatingScale *scale() const;
    int maxRating() const;
    QString alternative(int num) const;
    QString criterion(int num) const;

    int criteriaCount() const;
    int alternativeCount() const;

    CriteriaMatrix *criteriaMatrix() const;
    AlternativesMatrix *alternativesMatrix(int numberCriteria) const;    
    QStringListModel *criteriaModel();
    QStringListModel *alternativesModel();
    EntringDataModel *entringModel();

    ResultMatrix *resultMatrix() const;
    void setResult(ResultMatrix *resultMatrix);

private slots:
    void onCriteriaChanged(QModelIndex first, QModelIndex last);
    void onAlternativeChanged(QModelIndex first, QModelIndex last);

public slots:    
    void setCriteriaCount(int criteriaCount);
    void setAlternativeCount(int alternativeCount);
    void setMaxRating(int maxRating);

signals:
    void criteriaMatrixChanged(CriteriaMatrix *matrix);
    void alternativeMatrixChanged(int numberCriteria, AlternativesMatrix *matrix);    
    void criteriaCountChanged(int value);
    void alternativeCountChanged(int value);    
    void alternativeNameChanged(int numberCriteria, QString name);
    void dataChanged();
    void maxRatingChanged(int maxRating);
};

#endif // ANALYTICHIEARCHYMODEL_H

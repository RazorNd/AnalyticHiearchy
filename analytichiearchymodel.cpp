#include "analytichiearchymodel.h"
#include "criteriamatrix.h"
#include "alternativesmatrix.h"
#include "resultmatrix.h"

double AnalyticHiearchyModel::_randomConsistency[11] = {0, 0, 0, 0.58, 0.9, 1.12, 1.24, 1.32, 1.41, 1.45, 1.49};

const RatingScale *AnalyticHiearchyModel::scale() const
{
    return &_scale;
}

QString AnalyticHiearchyModel::alternative(int num) const
{
    return _alternatives.data(_alternatives.index(num), Qt::DisplayRole).toString();
}

QString AnalyticHiearchyModel::criterion(int num) const
{    
    return _criteria.data(_criteria.index(num), Qt::DisplayRole).toString();
}


int AnalyticHiearchyModel::criteriaCount() const
{
    return _criteriaCount;
}

void AnalyticHiearchyModel::setCriteriaCount(int criteriaCount)
{
    _criteriaCount = criteriaCount;
    emit criteriaCountChanged(_criteriaCount);
    updateCriteria();
}

int AnalyticHiearchyModel::alternativeCount() const
{
    return _alternativeCount;
}

void AnalyticHiearchyModel::setAlternativeCount(int alternativeCount)
{
    _alternativeCount = alternativeCount;    
    emit alternativeCountChanged(_alternativeCount);
    updateAlternatives();
}

void AnalyticHiearchyModel::appendCriterias(int count)
{
    const int lastSize = _criteria.rowCount();
    const int currentSize = lastSize + count;
    _criteria.insertRows(lastSize, count);
    for(int i = lastSize; i < currentSize; i++)
    {
        _criteria.setData(_criteria.index(i), QString("Критерий %1").arg(i + 1));
    }
}

void AnalyticHiearchyModel::appendAlternatives(int count)
{
    const int lastSize = _alternatives.rowCount();
    const int currentSize = lastSize + count;
    _alternatives.insertRows(lastSize, count);
    for(int i = lastSize; i < currentSize; i++)
    {
        _alternatives.setData(_alternatives.index(i), QString("Альтернатива %1").arg(i + 1));
    }
}

void AnalyticHiearchyModel::removeLastCriterias(int count)
{
    _criteria.removeRows(_criteria.rowCount() - count, count);
}

void AnalyticHiearchyModel::removeLastAlternatives(int count)
{
    _alternatives.removeRows(_alternatives.rowCount() - count, count);
}

CriteriaMatrix *AnalyticHiearchyModel::criteriaMatrix() const
{
    return _criteriaMatrix;
}

AlternativesMatrix *AnalyticHiearchyModel::alternativesMatrix(int numberCriteria) const
{
    return _alternativesMatrixs[numberCriteria];
}


QStringListModel *AnalyticHiearchyModel::criteriaModel()
{
    return &_criteria;
}

QStringListModel *AnalyticHiearchyModel::alternativesModel()
{
    return &_alternatives;
}

void AnalyticHiearchyModel::onCriteriaChanged(QModelIndex first, QModelIndex last)
{
    for(int i = first.row(); i <= last.row(); i++)
    {
        AlternativesMatrix *matrix = _alternativesMatrixs.at(i);
        matrix->setCriteriaName(_criteria.index(i).data().toString());
    }
}

void AnalyticHiearchyModel::onAlternativeChanged(QModelIndex first, QModelIndex last)
{
    for(int i = first.row(); i <= last.row(); i++)
    {
        emit alternativeNameChanged(i, alternative(i));
    }
}


ResultMatrix *AnalyticHiearchyModel::resultMatrix() const
{
    return _result;
}

void AnalyticHiearchyModel::createAlternativesMatrixs()
{
    _alternativesMatrixs.reserve(criteriaCount());
    for(int i = _alternativesMatrixs.size(); i < criteriaCount(); i++)
    {
        AlternativesMatrix *matrix = new AlternativesMatrix(i, this);
        _alternativesMatrixs.push_back(matrix);
        emit alternativeMatrixChanged(i, matrix);
    }
}

void AnalyticHiearchyModel::updateAlternativesMatrixs()
{    
    if(_alternativesMatrixs.size() < criteriaCount())
        createAlternativesMatrixs();
    else
        deleteAlternativesMatrixs();
}

void AnalyticHiearchyModel::deleteAlternativesMatrixs()
{
    for(int i = _alternativesMatrixs.size(); i < criteriaCount(); i++)
    {
        AlternativesMatrix *matrix = _alternativesMatrixs[i];
        matrix->deleteLater();
    }
    _alternativesMatrixs.resize(criteriaCount());
}

void AnalyticHiearchyModel::updateCriteria()
{
    if(_criteriaCount == _criteria.rowCount())
        return;
    updateAlternativesMatrixs();
    if(_criteriaCount > _criteria.rowCount())
        appendCriterias(_criteriaCount - _criteria.rowCount());
    else
        removeLastCriterias(_criteria.rowCount() - _criteriaCount);
}

void AnalyticHiearchyModel::updateAlternatives()
{
    if(_alternativeCount == _alternatives.rowCount())
        return;
    if(_alternativeCount > _alternatives.rowCount())
        appendAlternatives(_alternativeCount - _alternatives.rowCount());
    else
        removeLastAlternatives(_alternatives.rowCount() - _alternativeCount);
}

AnalyticHiearchyModel::AnalyticHiearchyModel(QObject *parent):
    QObject(parent), _criteriaCount(0), _alternativeCount(0), _criteriaMatrix(0)
{
    _criteriaMatrix = new CriteriaMatrix(this);
    _result = new ResultMatrix(this);
    connect(&_criteria, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            SLOT(onCriteriaChanged(QModelIndex,QModelIndex)));
    connect(&_alternatives, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            SLOT(onAlternativeChanged(QModelIndex,QModelIndex)));
}

double AnalyticHiearchyModel::randomConsistency(int size)
{
    if(size > 10)
        return 0;
    return _randomConsistency[size];
}

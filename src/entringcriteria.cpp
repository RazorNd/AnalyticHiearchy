#include "entringcriteria.h"
#include "model/criteriamatrix.h"
#include <QHash>

//NOTE: убрать дебаг
#include <QDebug>


AnalyticHiearchyModel *EntringCriteria::model() const
{
    return _model;
}

void EntringCriteria::setModel(AnalyticHiearchyModel *model)
{
    _model = model;
    connect(model, SIGNAL(criteriaCountChanged(int)), SLOT(setCriteriaCount(int)));
    setCriteriaCount(model->criteriaCount());
}

void EntringCriteria::setCriteriaCount(int count)
{
    if(count == _criteriaCount)
        return;
    if(count > _criteriaCount)
    {
        increaseCriteriaCount(_criteriaCount, count);
    }
    else
    {
        reduceCriteriaCount(_criteriaCount, count);
    }
    _criteriaCount = count;
}

void EntringCriteria::enterCriteria()
{

    QVector<int> criteriaLevels(_criteriaCount);
    int currentLevel = 1;
    criteriaLevels[criterionNumber(0)] = currentLevel;
    for(int i = 1; i < _criteriaCount; i++)
    {
        if(_compare.at(i-1)->currentIndex() == Greater)
        {
            currentLevel++;
        }
        criteriaLevels[criterionNumber(i)] = currentLevel;
    }

    CriteriaMatrix *matrix = _model->criteriaMatrix();
    for(int i = 0; i < _criteriaCount; i++)
    {
        for(int j = i + 1; j < _criteriaCount; j++)
        {

            matrix->setData(matrix->index(i, j),
                            rating(criteriaLevels[i], criteriaLevels[j], currentLevel),
                            Qt::EditRole);
        }
    }
}

void EntringCriteria::increaseCriteriaCount(int lastCount, int newCount)
{
    _criterion.resize(newCount);
    _compare.resize(newCount - 1);
    for(int i = lastCount; i < newCount; i++)
    {
        if(i > 0)
        {
            _compare[i - 1] = createCompareComboBox();
        }
        _criterion[i] = createCriteriaComboBox();

    }
}

void EntringCriteria::reduceCriteriaCount(int lastCount, int newCount)
{
    for(int i = newCount; i < lastCount; i++)
    {
        if(i > 0)
        {
            QComboBox *compare = _compare.at(i - 1);
            _layout->removeWidget(compare);
            compare->deleteLater();
        }
        QComboBox *criterion = _criterion.at(i);
        _layout->removeWidget(criterion);
        criterion->deleteLater();
    }
    _criterion.resize(newCount);
}

QComboBox *EntringCriteria::createCriteriaComboBox()
{
    QComboBox *criterion = new QComboBox(this);
    _layout->addWidget(criterion);
    //TODO: сменить модель
    criterion->setModel(_model->criteriaModel());
    criterion->setCurrentIndex(-1);
    return criterion;
}

QComboBox *EntringCriteria::createCompareComboBox()
{
    QComboBox *compare = new QComboBox(this);
    _layout->addWidget(compare);
    compare->addItems(QStringList() << ">" << "\u2248");
    return compare;
}

int EntringCriteria::criterionNumber(int pos)
{
    if(pos >= _criteriaCount)
        return -1;
    return _criterion.at(pos)->currentIndex();
}

int EntringCriteria::rating(int first, int second, int maxLevel) const
{
    return (double)(second - first) * (_model->maxRating() - 1) / (maxLevel - 1);
}

EntringCriteria::EntringCriteria(QWidget *parent) : QWidget(parent), _criteriaCount(0)
{
    _layout = new QHBoxLayout;
    setLayout(_layout);
}



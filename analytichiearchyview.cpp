#include "analytichiearchyview.h"
#include "criteriamatrix.h"
#include "alternativesmatrix.h"
#include "resultmatrix.h"
#include <QListWidget>
#include <QStringListModel>


AnalyticHiearchyModel *AnalyticHiearchyView::model() const
{
    return _model;
}

void AnalyticHiearchyView::setModel(AnalyticHiearchyModel *model)
{
    _model = model;
    ResultMatrix *resultMatrix = model->resultMatrix();

    _resultView->setModel(resultMatrix);
    connect(_resultView, SIGNAL(updateData()), resultMatrix, SLOT(updateDate()));
    setConnectParam();
    update();
}

void AnalyticHiearchyView::onCriteriaChanged(CriteriaMatrix *matrix)
{
    _criteriaView->setModel(matrix);
}

void AnalyticHiearchyView::onAlternativeChanged(int numberCriteria, AlternativesMatrix *matrix)
{
    _alternativesViews.at(numberCriteria)->setModel(matrix);
}

void AnalyticHiearchyView::onCriteriaCountChanged(int count)
{
    if(count == _alternativesViews.size())
        return;
    if(count > _alternativesViews.size())
    {
        _alternativesViews.reserve(count);
        for(int i = _alternativesViews.size(); i < count; i++)
        {
            PrioritizationMatrixView *view = new PrioritizationMatrixView(i + 2,QString() ,this);
            connect(view, SIGNAL(nameChaned(int,QString)), SLOT(onTabNameChanged(int,QString)));
            _alternativesViews.push_back(view);
        }
        return;
    }
    for(int i = count; i < _alternativesViews.size(); i++)
    {
        PrioritizationMatrixView *view = _alternativesViews.at(i);
        removeTab(view->numberTab());
        view->deleteLater();
    }
    _alternativesViews.resize(count);
}

void AnalyticHiearchyView::onTabNameChanged(int numberTab, QString name)
{
    this->setTabText(numberTab, name);
}

void AnalyticHiearchyView::update()
{
    onCriteriaCountChanged(_model->alternativeCount());
    onCriteriaChanged(model()->criteriaMatrix());
    for(int i = 0; i < model()->alternativeCount(); i++)
    {
        onAlternativeChanged(i, model()->alternativesMatrix(i));
    }
}

void AnalyticHiearchyView::setConnectParam()
{
    param->setModel(_model);
    connect(param, SIGNAL(criteriaCountChanged(int)),
            _model, SLOT(setCriteriaCount(int)));
    connect(param, SIGNAL(alternativesCountChanged(int)),
            _model, SLOT(setAlternativeCount(int)));    
    connect(_model, SIGNAL(criteriaCountChanged(int)), SLOT(onCriteriaCountChanged(int)));
    connect(_model, SIGNAL(alternativeMatrixChanged(int,AlternativesMatrix*)),
            SLOT(onAlternativeChanged(int,AlternativesMatrix*)));
}

AnalyticHiearchyView::AnalyticHiearchyView(QWidget *parent):
    QTabWidget(parent)
{
    param = new ParamForm(this);
    addTab(param, "Параметры");    

    _criteriaView = new PrioritizationMatrixView("Критерии", this);

    _resultView = new ResultView(this);
    addTab(_resultView, "Результат");
}

#ifndef ANALYTICHIEARCHYVIEW_H
#define ANALYTICHIEARCHYVIEW_H

#include "../model/analytichiearchymodel.h"
#include "prioritizationmatrixview.h"
#include "resultview.h"
#include "../paramform.h"
#include <QTabWidget>
#include <QTableView>
#include <QVector>

class PrioritizationMatrixView;

class AnalyticHiearchyView : public QTabWidget
{
    Q_OBJECT    
    PrioritizationMatrixView *_criteriaView;
    QVector<PrioritizationMatrixView *> _alternativesViews;
    AnalyticHiearchyModel *_model;
    ParamForm *param;
    ResultView *_resultView;

    void setConnectParam();
public:
    AnalyticHiearchyView(QWidget *parent = 0);    

    AnalyticHiearchyModel *model() const;
    void setModel(AnalyticHiearchyModel *model);

public slots:
    void onCriteriaChanged(CriteriaMatrix *matrix);
    void onAlternativeChanged(int numberCriteria, AlternativesMatrix *matrix);
    void onCriteriaCountChanged(int count);

    void onTabNameChanged(int numberTab, QString name);
    void update();
};

#endif // ANALYTICHIEARCHYVIEW_H

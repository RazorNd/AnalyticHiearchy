#include "rankingresult.h"
#include "ui_rankingresult.h"
#include "../model/resultmatrix.h"
#include <QVector>
#include <QPair>

RankingResult::RankingResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RankingResult)
{
    ui->setupUi(this);
}

RankingResult::~RankingResult()
{
    delete ui;
}

void RankingResult::setModel(AnalyticHiearchyModel *model)
{
    _model = model;
    connect(model, SIGNAL(dataChanged()), SLOT(rankResult()));
    connect(model, SIGNAL(alternativeNameChanged(int,QString)), SLOT(rankResult()));
}

void RankingResult::rankResult()
{
    ui->rankedList->clear();    
    QVector<QPair<double, QString> > alternative(_model->alternativeCount());
    for(int i = 0; i < _model->alternativeCount(); i++)
    {
        alternative[i] = qMakePair(_model->resultMatrix()->globalPriority(i),
                                   _model->alternative(i));
    }
    qSort(alternative.begin(), alternative.end(), qGreater<QPair<double, QString> >());
    for(QVector<QPair<double, QString> >::const_iterator it = alternative.begin();
        it != alternative.end(); it++)
    {
        ui->rankedList->addItem(it->second);
    }
}

#include "paramform.h"
#include "ui_paramform.h"
#include "model/entringdatamodel.h"

ParamForm::ParamForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParamForm)
{
    ui->setupUi(this);    
}

ParamForm::~ParamForm()
{
    delete ui;
}

void ParamForm::setModel(AnalyticHiearchyModel *model)
{
    ui->tableView->setModel(model->entringModel());

    ui->maxRating->setValue(model->maxRating());

    connect(ui->criteriaCount, SIGNAL(valueChanged(int)),
            model, SLOT(setCriteriaCount(int)));
    connect(ui->alternativesCount, SIGNAL(valueChanged(int)),
            model, SLOT(setAlternativeCount(int)));
    connect(ui->maxRating, SIGNAL(valueChanged(int)),
            model, SLOT(setMaxRating(int)));

    ui->criteriaCount->setValue(2);
    ui->alternativesCount->setValue(3);
}

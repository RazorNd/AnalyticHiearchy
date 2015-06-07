#include "paramform.h"
#include "ui_paramform.h"

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
    ui->criteriaView->setModel(model->criteriaModel());
    ui->alternativesView->setModel(model->alternativesModel());

    ui->maxRating->setValue(model->maxRating());

    connect(ui->criteriaCount, SIGNAL(valueChanged(int)),
            model, SLOT(setCriteriaCount(int)));
    connect(ui->alternativesCount, SIGNAL(valueChanged(int)),
            model, SLOT(setAlternativeCount(int)));
    connect(ui->maxRating, SIGNAL(valueChanged(int)),
            model, SLOT(setMaxRating(int)));
}

#include "paramform.h"
#include "ui_paramform.h"

ParamForm::ParamForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParamForm)
{
    ui->setupUi(this);

    connect(ui->criteriaCount, SIGNAL(valueChanged(int)), SIGNAL(criteriaCountChanged(int)));
    connect(ui->alternativesCount, SIGNAL(valueChanged(int)), SIGNAL(alternativesCountChanged(int)));    
}

ParamForm::~ParamForm()
{
    delete ui;
}

void ParamForm::setModel(AnalyticHiearchyModel *model)
{
    ui->criteriaView->setModel(model->criteriaModel());
    ui->alternativesView->setModel(model->alternativesModel());
}

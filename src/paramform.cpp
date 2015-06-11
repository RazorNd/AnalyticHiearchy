#include "paramform.h"
#include "ui_paramform.h"
#include "model/entringdatamodel.h"
#include "criteriadirectiondeligate.h"

ParamForm::ParamForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParamForm)
{
    ui->setupUi(this);    
    ui->tableView->setItemDelegateForRow(0, new CriteriaDirectionDeligate(this));
}

ParamForm::~ParamForm()
{
    delete ui;
}

void ParamForm::setModel(AnalyticHiearchyModel *model)
{
    ui->tableView->setModel(model->entringModel());    
}

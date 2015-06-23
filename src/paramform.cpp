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
    connect(ui->submitButton, SIGNAL(clicked()),
            ui->entringCriteria, SLOT(enterCriteria()));
}

ParamForm::~ParamForm()
{
    delete ui;
}

void ParamForm::setModel(AnalyticHiearchyModel *model)
{
    ui->tableView->setModel(model->entringModel());
    EntringDataModel::EntringDataSelectionModel *selectionModel =
            new EntringDataModel::EntringDataSelectionModel(model->entringModel(), this);
    ui->tableView->setSelectionModel(selectionModel);
    ui->entringCriteria->setModel(model);
    connect(ui->submitButton, SIGNAL(clicked()),
            model->entringModel(), SLOT(enterModel()));
    connect(ui->clearButton, SIGNAL(clicked()),
            selectionModel, SLOT(deleteSelected()));
}

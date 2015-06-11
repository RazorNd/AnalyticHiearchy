#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setModel(AnalyticHiearchyModel *model)
{
    ui->maxRating->setValue(model->maxRating());
    ui->criteriaCount->setValue(model->criteriaCount());
    ui->alternativesCount->setValue(model->alternativeCount());

    ui->criteriasName->setModel(model->criteriaModel());
    ui->alternativesName->setModel(model->alternativesModel());

    connect(ui->maxRating, SIGNAL(valueChanged(int)), model, SLOT(setMaxRating(int)));
    connect(ui->criteriaCount, SIGNAL(valueChanged(int)), model, SLOT(setCriteriaCount(int)));
    connect(ui->alternativesCount, SIGNAL(valueChanged(int)), model, SLOT(setAlternativeCount(int)));
}

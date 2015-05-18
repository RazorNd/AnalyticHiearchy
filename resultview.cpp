#include "resultview.h"
#include "ui_resultview.h"

ResultView::ResultView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultView)
{
    ui->setupUi(this);
}

ResultView::~ResultView()
{
    delete ui;
}

void ResultView::setModel(ResultMatrix *model)
{
    ui->result->setModel(model);
    connect(model, SIGNAL(bestAlternativeChanged(QString)), ui->bestAlternative, SLOT(setText(QString)));
}

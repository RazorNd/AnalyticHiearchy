#include "resultview.h"
#include "ui_resultview.h"


ResultView::ResultView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultView)
{
    ui->setupUi(this);    
    sortModel = new QSortFilterProxyModel(this);
    ui->result->setModel(sortModel);
}

ResultView::~ResultView()
{
    delete ui;
}

void ResultView::setModel(ResultMatrix *model)
{
    sortModel->setSourceModel(model);

    connect(model, SIGNAL(bestAlternativeChanged(QString)), ui->bestAlternative, SLOT(setText(QString)));
}

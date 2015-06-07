#include "prioritizationmatrixview.h"
#include "ui_prioritizationmatrixview.h"
#include "../model/alternativesmatrix.h"
#include <QHBoxLayout>


int PrioritizationMatrixView::numberTab() const
{
    return _numberTab;
}

void PrioritizationMatrixView::setNumberTab(int number)
{
    _numberTab = number;
}

PrioritizationMatrixView::PrioritizationMatrixView(QString name, AnalyticHiearchyView *parent):
    QWidget(parent), _numberTab(-1), ui(new Ui::PrioritizationMatrixView)
{    
    ui->setupUi(this);
    delegate = new RatingEditDelegate(this);
    ui->view->setItemDelegate(delegate);
    _numberTab = parent->addTab(this, name);
}

PrioritizationMatrixView::PrioritizationMatrixView(int numberTab, QString name, AnalyticHiearchyView *parent):
    QWidget(parent), _numberTab(numberTab), ui(new Ui::PrioritizationMatrixView)
{
    ui->setupUi(this);
    delegate = new RatingEditDelegate(this);
    ui->view->setItemDelegate(delegate);
    parent->insertTab(numberTab, this, name);
}

PrioritizationMatrixView::~PrioritizationMatrixView()
{
    delete ui;
}

void PrioritizationMatrixView::setModel(AbstractPrioritizationMatrix *matrix)
{
    ui->view->setModel(matrix);
    delegate->setScale(matrix->scale());
    connect(matrix, SIGNAL(harmonizationChanged(double)),
            ui->harmonizationView, SLOT(setNum(double)));
    connect(ui->clearButton, SIGNAL(clicked()),
            matrix, SLOT(clear()));
    AlternativesMatrix *alternative = qobject_cast<AlternativesMatrix *>(matrix);
    if(alternative)
    {
        connect(alternative, SIGNAL(criteriaNameChanged(QString)), SLOT(setName(QString)));
    }
}

void PrioritizationMatrixView::setName(QString name)
{
    ui->name->setText(name);
    if(_numberTab != -1)
        emit nameChaned(_numberTab, name);
}

AbstractPrioritizationMatrix *PrioritizationMatrixView::model() const
{
    return qobject_cast<AbstractPrioritizationMatrix *>(ui->view->model());
}


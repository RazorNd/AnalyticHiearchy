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
    connect(this, SIGNAL(nameChaned(int,QString)), parent, SLOT(onTabNameChanged(int,QString)));
}

PrioritizationMatrixView::PrioritizationMatrixView(int numberTab, QString name, AnalyticHiearchyView *parent):
    QWidget(parent), _numberTab(numberTab), ui(new Ui::PrioritizationMatrixView)
{
    ui->setupUi(this);
    delegate = new RatingEditDelegate(this);
    ui->view->setItemDelegate(delegate);
    parent->insertTab(numberTab, this, name);
    connect(this, SIGNAL(nameChaned(int,QString)), parent, SLOT(onTabNameChanged(int,QString)));
}

PrioritizationMatrixView::~PrioritizationMatrixView()
{
    delete ui;
}

void PrioritizationMatrixView::setModel(AbstractPrioritizationMatrix *matrix)
{
    ui->view->setModel(matrix);    
    setName(matrix->getName());
    delegate->setScale(matrix->scale());
    connect(matrix, SIGNAL(harmonizationChanged(QString)),
            ui->harmonizationView, SLOT(setText(QString)));
    connect(matrix, SIGNAL(harmonizationColorChanged(QString)),
            ui->harmonizationView, SLOT(setStyleSheet(QString)));
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
    ui->name->setText(QString("МПС (%1)").arg(name));
    if(_numberTab != -1)
        emit nameChaned(_numberTab, QString("МПС (%1)").arg(name));
}

AbstractPrioritizationMatrix *PrioritizationMatrixView::model() const
{
    return qobject_cast<AbstractPrioritizationMatrix *>(ui->view->model());
}


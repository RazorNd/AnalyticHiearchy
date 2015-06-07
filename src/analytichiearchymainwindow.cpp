#include "analytichiearchymainwindow.h"
#include "ui_analytichiearchymainwindow.h"
#include <QMessageBox>
#include <QFile>

AnalyticHiearchyMainWindow::AnalyticHiearchyMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalyticHiearchyMainWindow)
{
    ui->setupUi(this);
    model = new AnalyticHiearchyModel(this);
    ui->view->setModel(model);

    QFile fileInfo(":/text/text/help.html");

    fileInfo.open(QFile::ReadOnly);
    helpMessage = new QMessageBox(QMessageBox::NoIcon, "Помошь",
                                          QString(fileInfo.readAll()), QMessageBox::Ok, this);
    fileInfo.close();
    fileInfo.setFileName(":/text/text/about.html");
    fileInfo.open(QFile::ReadOnly);

    aboutText = fileInfo.readAll();

    fileInfo.close();
}

AnalyticHiearchyMainWindow::~AnalyticHiearchyMainWindow()
{
    delete ui;
}

void AnalyticHiearchyMainWindow::help()
{    
    helpMessage->show();
}

void AnalyticHiearchyMainWindow::about()
{
    QMessageBox::about(this, "Метод анализа иерархий", aboutText);
}
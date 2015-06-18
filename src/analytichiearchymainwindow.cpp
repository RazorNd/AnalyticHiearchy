#include "analytichiearchymainwindow.h"
#include "ui_analytichiearchymainwindow.h"
#include "util/analytichiearchyexcelsaver.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>

AnalyticHiearchyMainWindow::AnalyticHiearchyMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalyticHiearchyMainWindow)
{
    ui->setupUi(this);
    model = new AnalyticHiearchyModel(this);
    model->setCriteriaCount(2);
    model->setAlternativeCount(3);
    ui->view->setModel(model);

    settings = new SettingsDialog(this);
    settings->setModel(model);

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

void AnalyticHiearchyMainWindow::showSettings()
{
    settings->open();
}

void AnalyticHiearchyMainWindow::exportsExcel()
{
    AnalyticHiearchyExcelSaver saver;

    /*QString fileName = QFileDialog::getSaveFileName(this, "Экспорт в Excel",
                                                    QString(),
                                                    "Excel (*.xlsx)", 0,
                                                    QFileDialog::DontConfirmOverwrite);
    if(fileName.isEmpty())
        return;*/
    saver.setFileName("example.xlsx");
    saver.write(model);
}

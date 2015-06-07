#ifndef ANALYTICHIEARCHYMAINWINDOW_H
#define ANALYTICHIEARCHYMAINWINDOW_H

#include "model/analytichiearchymodel.h"
#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
class AnalyticHiearchyMainWindow;
}

class AnalyticHiearchyMainWindow : public QMainWindow
{
    Q_OBJECT
    AnalyticHiearchyModel *model;
    QMessageBox *helpMessage;
    QString aboutText;
public:
    explicit AnalyticHiearchyMainWindow(QWidget *parent = 0);
    ~AnalyticHiearchyMainWindow();

private slots:
    void help();
    void about();

private:
    Ui::AnalyticHiearchyMainWindow *ui;
};

#endif // ANALYTICHIEARCHYMAINWINDOW_H

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "model/analytichiearchymodel.h"
#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    void setModel(AnalyticHiearchyModel *model);

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H

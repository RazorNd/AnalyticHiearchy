#ifndef PARAMFORM_H
#define PARAMFORM_H

#include "model/analytichiearchymodel.h"
#include <QWidget>

namespace Ui {
class ParamForm;
}

class ParamForm : public QWidget
{
    Q_OBJECT

public:
    explicit ParamForm(QWidget *parent = 0);
    ~ParamForm();

    void setModel(AnalyticHiearchyModel *model);
private:
    Ui::ParamForm *ui;
};

#endif // PARAMFORM_H

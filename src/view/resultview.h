#ifndef RESULTVIEW_H
#define RESULTVIEW_H

#include "../model/resultmatrix.h"
#include <QWidget>

namespace Ui {
class ResultView;
}

class ResultView : public QWidget
{
    Q_OBJECT

public:
    explicit ResultView(QWidget *parent = 0);
    ~ResultView();

    void setModel(ResultMatrix *model);

signals:
    void updateData();

private:
    Ui::ResultView *ui;
};

#endif // RESULTVIEW_H

#ifndef RESULTVIEW_H
#define RESULTVIEW_H

#include "../model/resultmatrix.h"
#include <QWidget>
#include <QSortFilterProxyModel>

namespace Ui {
class ResultView;
}

class ResultView : public QWidget
{
    Q_OBJECT
    QSortFilterProxyModel *sortModel;
public:
    explicit ResultView(QWidget *parent = 0);
    ~ResultView();

    void setModel(ResultMatrix *model);

private:
    Ui::ResultView *ui;
};

#endif // RESULTVIEW_H

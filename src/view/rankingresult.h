#ifndef RANKINGRESULT_H
#define RANKINGRESULT_H

#include "../model/analytichiearchymodel.h"
#include <QWidget>

namespace Ui {
class RankingResult;
}

class RankingResult : public QWidget
{
    Q_OBJECT
    AnalyticHiearchyModel *_model;
public:
    explicit RankingResult(QWidget *parent = 0);
    ~RankingResult();

    void setModel(AnalyticHiearchyModel *model);

private slots:
    void rankResult();

private:
    Ui::RankingResult *ui;
};

#endif // RANKINGRESULT_H

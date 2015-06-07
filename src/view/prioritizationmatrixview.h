#ifndef PRIORITIZATIONMATRIXVIEW_H
#define PRIORITIZATIONMATRIXVIEW_H

#include "../model/abstractprioritizationmatrix.h"
#include "../ratingeditdelegate.h"
#include "analytichiearchyview.h"
#include <QWidget>
#include <QTableView>

namespace Ui {
class PrioritizationMatrixView;
}

class AnalyticHiearchyView;

class PrioritizationMatrixView : public QWidget
{
    Q_OBJECT
    int _numberTab;    
    RatingEditDelegate *delegate;
public:
    explicit PrioritizationMatrixView(QString name, AnalyticHiearchyView *parent);
    explicit PrioritizationMatrixView(int numberTab, QString name, AnalyticHiearchyView *parent);
    ~PrioritizationMatrixView();
    AbstractPrioritizationMatrix *model() const;
    int numberTab() const;
    void setNumberTab(int number);
signals:    
    void nameChaned(int numberTab, QString name);
public slots:
    void setModel(AbstractPrioritizationMatrix *matrix);
    void setName(QString name);

private:
    Ui::PrioritizationMatrixView *ui;
};

#endif // PRIORITIZATIONMATRIXVIEW_H

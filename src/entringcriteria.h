#ifndef ENTRINGCRITERIA_H
#define ENTRINGCRITERIA_H

#include "model/analytichiearchymodel.h"
#include <QWidget>
#include <QVector>
#include <QComboBox>
#include <QBoxLayout>

class EntringCriteria : public QWidget
{
    Q_OBJECT

    enum Compare{Greater = 0, Equally = 1};

    AnalyticHiearchyModel *_model;
    int _criteriaCount;
    QVector<QComboBox *> _criterion;
    QVector<QComboBox *> _compare;
    QHBoxLayout *_layout;

    void increaseCriteriaCount(int lastCount, int newCount);
    void reduceCriteriaCount(int lastCount, int newCount);

    QComboBox *createCriteriaComboBox();
    QComboBox *createCompareComboBox();

    int criterionNumber(int pos);
    int rating(int first, int second, int maxLevel) const;
public:
    EntringCriteria(QWidget *parent = 0);

    AnalyticHiearchyModel *model() const;
    void setModel(AnalyticHiearchyModel *model);
private slots:
    void setCriteriaCount(int count);
public slots:
    void enterCriteria();
};



#endif // ENTRINGCRITERIA_H

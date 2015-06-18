#ifndef ANALYTICHIEARCHYEXCELEXPORTER_H
#define ANALYTICHIEARCHYEXCELEXPORTER_H

#include "../model/analytichiearchymodel.h"
#include <QObject>
#include <QAxObject>

class SetCellCallBack
{
public:
    static bool setValueCell(QAxObject *cell, const QVariant &value);
    virtual bool operator()
    (QAxObject *cell, const QVariant &value) const;
};

class StyledSetCellCallBack : public SetCellCallBack
{
    // SetCellCallBack interface
    QAxObject *style;
public:
    StyledSetCellCallBack(QAxObject *workbook);
    bool operator ()(QAxObject *cell, const QVariant &value) const;
};

class AnalyticHiearchyExcelExporter : public QObject
{
    Q_OBJECT

    AnalyticHiearchyModel *_model;

    bool processingWorkbook(QAxObject *workbook);
    bool savingEntringModel(QAxObject *sheet);
    bool savingCriteriaModel(QAxObject *sheet, const StyledSetCellCallBack &callback);
    bool savingAlternativeModel(int criteria, QAxObject *sheet, const StyledSetCellCallBack &callback);
    bool savingResultModel(QAxObject *sheet);

    bool writeSheet(QAxObject *sheet,
                    QAbstractItemModel *model,
                    const SetCellCallBack& callback = SetCellCallBack());
    bool writeCellVallue(QAxObject *cell, const QVariant &value);
    QAxObject *getSheet(QAxObject *workbook, int number);
public:
    AnalyticHiearchyExcelExporter(QObject *parent = 0);

    AnalyticHiearchyModel *model() const;
    void setModel(AnalyticHiearchyModel *model);

    bool exportModel(AnalyticHiearchyModel *model);
    bool exportModel();
};

#endif // ANALYTICHIEARCHYEXCELEXPORTER_H

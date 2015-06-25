#ifndef ANALYTICHIEARCHYEXCELEXPORTER_H
#define ANALYTICHIEARCHYEXCELEXPORTER_H

#include "../model/analytichiearchymodel.h"
#include <QObject>
#include <QAxObject>
#include <QColor>

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

    bool processingWorkbook(QAxObject *workbook, bool full = true);

    bool savingParam(QAxObject *sheet);

    bool savingEntringModel(QAxObject *sheet);
    bool savingCriteriaModel(QAxObject *sheet, const StyledSetCellCallBack &callback);
    bool savingAlternativeModel(int criteria, QAxObject *sheet, const StyledSetCellCallBack &callback);
    bool savingResultModel(QAxObject *sheet);   

    bool writeSheet(QAxObject *sheet,
                    QAbstractItemModel *model,
                    int shiftColumn = 0,
                    int shiftRow = 0,
                    const SetCellCallBack& callback = SetCellCallBack());
    bool writeCellVallue(QAxObject *cell, const QVariant &value);
    QAxObject *getSheet(QAxObject *workbook, int number);

    void autoFit(QAxObject *sheet);
    void setBorder(QAxObject *sheet, const QString &range, bool medium = false);
    void setColor(QAxObject *sheet, const QString &range, QColor color);

    void mergeCell(QAxObject *sheet, const QString &range);
public:
    AnalyticHiearchyExcelExporter(QObject *parent = 0);

    AnalyticHiearchyModel *model() const;
    void setModel(AnalyticHiearchyModel *model);

    bool exportModel(bool full = true);

public slots:
    void createShortReport();
    void createFullReport();
};

#endif // ANALYTICHIEARCHYEXCELEXPORTER_H

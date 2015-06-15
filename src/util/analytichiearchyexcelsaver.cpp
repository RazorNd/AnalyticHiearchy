#include "analytichiearchyexcelsaver.h"
#include "../model/entringdatamodel.h"
#include "excel.h"
#include <QDebug>

QString AnalyticHiearchyExcelSaver::fileName() const
{
    return _fileName;
}

void AnalyticHiearchyExcelSaver::setFileName(const QString &fileName)
{
    _fileName = fileName;
}

QString AnalyticHiearchyExcelSaver::format() const
{
    return _format;
}

void AnalyticHiearchyExcelSaver::setFormat(const QString &format)
{
    _format = format;
}

bool AnalyticHiearchyExcelSaver::write(AnalyticHiearchyModel *model)
{
    if(_fileName.isEmpty())
        return false;
    QAxObject excelApp("Excel.Application");
    QAxObject *workbook = excelApp.querySubObject("Workbooks")
            ->querySubObject("add()");
    QAxObject *sheet = workbook->querySubObject("Sheets")
            ->querySubObject("Item(int)", 1);
    sheet->setProperty("Name", "Входящие данные");
    writeSheet(sheet, model->entringModel());
    QVariant saved = workbook->dynamicCall("SaveAs(QString)", _fileName);
    qDebug() << saved;
    excelApp.dynamicCall("Quit()");
}

void AnalyticHiearchyExcelSaver::writeSheet(QAxObject *sheet, QAbstractItemModel *model)
{
    if(sheet->isNull())
        return;
    int iSize = model->rowCount(),
            jSize = model->columnCount();
    for(int i = 0; i < iSize; i++)
    {
        for(int j = 0; j < jSize; j++)
        {
            writeCell(sheet->querySubObject("Cells(int, int)", i + 1, j + 1),
                      model->index(i, j).data());
        }
    }
}

void AnalyticHiearchyExcelSaver::writeCell(QAxObject *cell, const QVariant &value)
{
    if(cell->isNull())
        return;
    cell->setProperty("Value", value);
}

AnalyticHiearchyExcelSaver::AnalyticHiearchyExcelSaver(QObject *parent) : QObject(parent)
{
}



#include "analytichiearchyexcelexporter.h"
#include "../model/entringdatamodel.h"
#include "../model/criteriamatrix.h"
#include "../model/alternativesmatrix.h"
#include "../model/resultmatrix.h"
#include <QDebug>

bool AnalyticHiearchyExcelExporter::exportModel(AnalyticHiearchyModel *model)
{
    setModel(model);
    return exportModel();
}

bool AnalyticHiearchyExcelExporter::exportModel()
{    
    if(!_model)
        return false;
    QAxObject excelApp("Excel.Application");
    int needsWorksheets = 3 + _model->criteriaCount();
    excelApp.setProperty("SheetsInNewWorkbook", needsWorksheets);

    QAxObject *workbook = excelApp.querySubObject("Workbooks")
            ->querySubObject("add()");

    processingWorkbook(workbook);


    excelApp.setProperty("Visible", true);
    return true;
}

AnalyticHiearchyModel *AnalyticHiearchyExcelExporter::model() const
{
    return _model;
}

void AnalyticHiearchyExcelExporter::setModel(AnalyticHiearchyModel *model)
{
    _model = model;
}


QAxObject * AnalyticHiearchyExcelExporter::getSheet(QAxObject *workbook, int number)
{
    if(!workbook)
        return 0;
    QAxObject *sheets = workbook->querySubObject("Sheets");
    if(!sheets)
        return 0;
    QAxObject *sheet = sheets->querySubObject("Item(int)", number);

    return sheet;
}

bool AnalyticHiearchyExcelExporter::processingWorkbook(QAxObject *workbook)
{    
    if(!workbook)
        return false;
    StyledSetCellCallBack callBack(workbook);
    if(!savingEntringModel(getSheet(workbook, 1)))
    {
        //TODO: add error not saving Entring Model
        return false;        
    }
    if(!savingCriteriaModel(getSheet(workbook, 2), callBack))
    {
        //TODO: add error not saving Criteria Model
        return false;
    }
    int criteriaCount = _model->criteriaCount();
    for(int i = 0; i < criteriaCount; i++)
    {
        if(!savingAlternativeModel(i, getSheet(workbook, 3 + i), callBack))
        {
            //TODO: add error not saving Alternative Model
            return false;
        }
    }
    if(!savingResultModel(getSheet(workbook, 3 + criteriaCount)))
    {
        //TODO: add error not saving Result Model
        return false;
    }
    return true;
}

bool AnalyticHiearchyExcelExporter::savingEntringModel(QAxObject *sheet)
{
    if(!sheet)
        return false;
    return sheet->setProperty("Name", "Входящие данные") &&
            writeSheet(sheet, _model->entringModel());
}

bool AnalyticHiearchyExcelExporter::savingCriteriaModel(QAxObject *sheet, const StyledSetCellCallBack &callback)
{
    if(!sheet)
        return false;
    return sheet->setProperty("Name", "Критерии") &&
            writeSheet(sheet, _model->criteriaMatrix(), callback);

}

bool AnalyticHiearchyExcelExporter::savingAlternativeModel(int criteria, QAxObject *sheet,  const StyledSetCellCallBack &callback)
{
    if(!sheet)
        return false;
    return sheet->setProperty("Name", _model->criterion(criteria)) &&
            writeSheet(sheet, _model->alternativesMatrix(criteria), callback);
}

bool AnalyticHiearchyExcelExporter::savingResultModel(QAxObject *sheet)
{
    if(!sheet)
        return false;
    return sheet->setProperty("Name", "Результат") &&
            writeSheet(sheet, _model->resultMatrix());
}

bool AnalyticHiearchyExcelExporter::writeSheet(QAxObject *sheet, QAbstractItemModel *model, const SetCellCallBack &callback)
{
    if(!sheet)
        return false;
    int iSize = model->rowCount(),
            jSize = model->columnCount();
    bool success = true;
    for(int i = 0; i < iSize && success; i++)
    {
        success &= writeCellVallue(sheet->querySubObject("Cells(int, int)",
                                                   i + 2, 1),
                             model->headerData(i, Qt::Vertical));
        for(int j = 0; j < jSize && success; j++)
        {
            success &= callback(sheet->querySubObject("Cells(int, int)", i + 2, j + 2),
                                model->index(i, j).data());
        }
    }
    for(int j = 0; j < jSize && success; j++)
    {
        success &= writeCellVallue(sheet->querySubObject("Cells(int, int)",
                                                   1, j + 2),
                             model->headerData(j, Qt::Horizontal));
    }
    return success;
}

bool AnalyticHiearchyExcelExporter::writeCellVallue(QAxObject *cell,
                                                 const QVariant &value)
{
    if(!cell)
        return false;    

    if(value.isValid())
        return cell->setProperty("Value", value);
    return true;
}

AnalyticHiearchyExcelExporter::AnalyticHiearchyExcelExporter(QObject *parent) : QObject(parent)
{
}

bool SetCellCallBack::setValueCell(QAxObject *cell, const QVariant &value)
{
    if(!cell)
        return false;
    if(value.isValid())
        return cell->setProperty("Value", value);
    return true;
}

bool SetCellCallBack::operator()(QAxObject *cell, const QVariant &value) const
{
    return setValueCell(cell, value);
}


StyledSetCellCallBack::StyledSetCellCallBack(QAxObject *workbook)
{
    if(!workbook)
        return;
    QAxObject *styles = workbook->querySubObject("Styles");
    if(!styles)
        return;
    style = styles->querySubObject("Add(QString)", "devided");
    style->setProperty("NumberFormat", "#\" \"?/?");
}

bool StyledSetCellCallBack::operator ()(QAxObject *cell, const QVariant &value) const
{
    if(!cell)
        return false;
    QRegExp regExp("\\d/\\d");
    if(value.toString().contains(regExp))
        cell->setProperty("Style", style->asVariant());
    return SetCellCallBack::operator ()(cell, value);
}

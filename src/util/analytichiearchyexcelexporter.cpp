#include "analytichiearchyexcelexporter.h"
#include "../model/entringdatamodel.h"
#include "../model/criteriamatrix.h"
#include "../model/alternativesmatrix.h"
#include "../model/resultmatrix.h"
#include <QDebug>

bool AnalyticHiearchyExcelExporter::exportModel(bool full)
{    
    if(!_model)
        return false;
    QAxObject excelApp("Excel.Application");

    int needsWorksheets;
    if(full)
        needsWorksheets = 4 + _model->criteriaCount();
    else
        needsWorksheets = 3;

    excelApp.setProperty("SheetsInNewWorkbook", needsWorksheets);

    QAxObject *workbook = excelApp.querySubObject("Workbooks")
            ->querySubObject("add()");

    processingWorkbook(workbook, full);


    excelApp.setProperty("Visible", true);
    return true;
}

void AnalyticHiearchyExcelExporter::createShortReport()
{
    exportModel(false);
}

void AnalyticHiearchyExcelExporter::createFullReport()
{
    exportModel();
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

bool AnalyticHiearchyExcelExporter::processingWorkbook(QAxObject *workbook, bool full)
{    
    if(!workbook)
        return false;
    StyledSetCellCallBack callBack(workbook);
    int pageNumber = 1;
    if(!savingParam(getSheet(workbook, pageNumber++)))
    {
        //TODO: add error not saving param
        return false;
    }
    if(!savingEntringModel(getSheet(workbook, pageNumber++)))
    {
        //TODO: add error not saving Entring Model
        return false;        
    }
    if(full)
    {
        if(!savingCriteriaModel(getSheet(workbook, pageNumber++), callBack))
        {
            //TODO: add error not saving Criteria Model
            return false;
        }
        int criteriaCount = _model->criteriaCount();
        for(int i = 0; i < criteriaCount; i++)
        {
            if(!savingAlternativeModel(i, getSheet(workbook, pageNumber++), callBack))
            {
                //TODO: add error not saving Alternative Model
                return false;
            }
        }
    }
    if(!savingResultModel(getSheet(workbook, pageNumber++)))
    {
        //TODO: add error not saving Result Model
        return false;
    }
    return true;
}

void AnalyticHiearchyExcelExporter::autoFit(QAxObject *sheet)
{
    QAxObject *columns = sheet->querySubObject("Columns(QString)", "A:Z");
    if(columns)
    {
        columns->dynamicCall("AutoFit()");
    }
}

void AnalyticHiearchyExcelExporter::setBorder(QAxObject *sheet, const QString &range, bool medium)
{
    QAxObject *columns = sheet->querySubObject("Range(QString)", range);
    if(columns)
    {
        if(medium)
            columns->dynamicCall("BorderAround(xlContinuous, xlMedium)");
        else
            columns->dynamicCall("BorderAround(xlContinuous)");
    }
}

void AnalyticHiearchyExcelExporter::setColor(QAxObject *sheet, const QString &range, QColor color)
{
    QAxObject *columns = sheet->querySubObject("Range(QString)", range);
    if(columns)
    {
        QAxObject *interior = columns->querySubObject("Interior");
        if(interior)
        {
            interior->setProperty("Color",color);
        }
    }
}

void AnalyticHiearchyExcelExporter::mergeCell(QAxObject *sheet, const QString &range)
{
    QAxObject *columns = sheet->querySubObject("Range(QString)", range);
    if(columns)
    {
        columns->setProperty("MergeCells", true);
    }
}

bool AnalyticHiearchyExcelExporter::savingParam(QAxObject *sheet)
{
    bool ok = sheet->setProperty("Name", "Параметры");
    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 1, 1),
                    "Колличество критериев");
    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 1, 2),
                    _model->criteriaCount());

    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 2, 1),
                    "Колличество альтернатив");
    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 2, 2),
                    _model->alternativeCount());

    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 3, 1),
                    "Максимальная оценка");
    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 3, 2),
                    _model->maxRating());
    setBorder(sheet, "A1:B3", true);

    int size = _model->criteriaCount();
    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 5, 1),
                    "Критерии:");
    for(int i = 0; i < size; i++)
    {
        ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 6 + i, 1),
                        _model->criterion(i));
    }
    setBorder(sheet, QString("A5:A%1").arg(5 + size), true);

    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 5, 3),
                    "Альтернативы:");
    size = _model->alternativeCount();
    for(int i = 0; i < size; i++)
    {
        ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 6 + i, 3),
                        _model->alternative(i));
    }
    setBorder(sheet, QString("C5:C%1").arg(5 + size), true);

    autoFit(sheet);
    setBorder(sheet, "A1:B3");
    return ok;
}

bool AnalyticHiearchyExcelExporter::savingEntringModel(QAxObject *sheet)
{
    if(!sheet)
        return false;    
    bool ok = sheet->setProperty("Name", "Исходные данные") &&
            writeSheet(sheet, _model->entringModel(), 1, 1);
    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 1, 1),
                    "Исходные данные");
    autoFit(sheet);
    setBorder(sheet, "B2", true);
    setBorder(sheet, QString("C3:%1%2").arg(QChar('B'+_model->entringModel()->columnCount()))
                                       .arg(_model->entringModel()->rowCount() + 2), true);
    setBorder(sheet, QString("B2:%1%2").arg(QChar('B'+_model->entringModel()->columnCount()))
                                       .arg(_model->entringModel()->rowCount() + 2), true);
    return ok;
}

bool AnalyticHiearchyExcelExporter::savingCriteriaModel(QAxObject *sheet, const StyledSetCellCallBack &callback)
{
    if(!sheet)
        return false;
    bool ok = sheet->setProperty("Name", "Критерии") &&
            writeSheet(sheet, _model->criteriaMatrix(), 1, 1,callback);
    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 1, 1),
                    "МПС Критерии");
    autoFit(sheet);

    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)",
                                                _model->criteriaMatrix()->rowCount() + 4,
                                                2),
                          "Отношение согласованности:");
    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)",
                                                _model->criteriaMatrix()->rowCount() + 4,
                                                4),
                          _model->criteriaMatrix()->harmonization());

    setBorder(sheet, "B2", true);
    setBorder(sheet, QString("C3:%1%2").arg(QChar('B'+_model->criteriaMatrix()->columnCount()))
                                       .arg(_model->criteriaMatrix()->rowCount() + 2), true);
    setBorder(sheet, QString("B2:%1%2").arg(QChar('B'+_model->criteriaMatrix()->columnCount()))
                                       .arg(_model->criteriaMatrix()->rowCount() + 2), true);
    setColor( sheet, QString("C3:%1%2").arg(QChar('B'+_model->criteriaMatrix()->columnCount() - 2))
                                       .arg(_model->criteriaMatrix()->rowCount() + 1), QColor(0x00CCFF));
    return ok;
}

bool AnalyticHiearchyExcelExporter::savingAlternativeModel(int criteria, QAxObject *sheet,  const StyledSetCellCallBack &callback)
{
    if(!sheet)
        return false;
    bool ok = sheet->setProperty("Name", _model->criterion(criteria)) &&
            writeSheet(sheet, _model->alternativesMatrix(criteria), 1, 1, callback);
    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 1, 1),
                          QString("МПС %1").arg(_model->criterion(criteria)));
    autoFit(sheet);

    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)",
                                                _model->alternativesMatrix(criteria)->rowCount() + 4,
                                                2),
                          "Отношение согласованности:");
    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)",
                                                _model->alternativesMatrix(criteria)->rowCount() + 4,
                                                4),
                          _model->alternativesMatrix(criteria)->harmonization());

    setBorder(sheet, "B2", true);
    setBorder(sheet, QString("C3:%1%2").arg(QChar('B'+_model->alternativesMatrix(criteria)->columnCount()))
                                       .arg(_model->alternativesMatrix(criteria)->rowCount() + 2), true);
    setBorder(sheet, QString("B2:%1%2").arg(QChar('B'+_model->alternativesMatrix(criteria)->columnCount()))
                                       .arg(_model->alternativesMatrix(criteria)->rowCount() + 2), true);
    setColor( sheet, QString("C3:%1%2").arg(QChar('B'+_model->alternativesMatrix(criteria)->columnCount() - 2))
                                       .arg(_model->alternativesMatrix(criteria)->rowCount() + 1), QColor(0x00CCFF));
    return ok;
}

bool AnalyticHiearchyExcelExporter::savingResultModel(QAxObject *sheet)
{
    if(!sheet)
        return false;
    bool ok = sheet->setProperty("Name", "Глобальные приоритеты") &&
            writeSheet(sheet, _model->resultMatrix(), 1, 1);
    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)", 1, 1),
                          "Глобальные приоритеты");

    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)",
                                                _model->resultMatrix()->rowCount() + 4,
                                                2),
                          "Лучшая альтернатива:");
    ok &= writeCellVallue(sheet->querySubObject("Cells(int, int)",
                                                _model->resultMatrix()->rowCount() + 4,
                                                3),
                          _model->resultMatrix()->bestAlternative());
    autoFit(sheet);
    setBorder(sheet, "B2", true);
    setBorder(sheet, QString("C3:%1%2").arg(QChar('B'+_model->resultMatrix()->columnCount()))
                                       .arg(_model->resultMatrix()->rowCount() + 2), true);
    setBorder(sheet, QString("B2:%1%2").arg(QChar('B'+_model->resultMatrix()->columnCount()))
                                       .arg(_model->resultMatrix()->rowCount() + 2), true);
    return ok;
}

bool AnalyticHiearchyExcelExporter::writeSheet(QAxObject *sheet,
                                               QAbstractItemModel *model,
                                               int shiftColumn,
                                               int shiftRow,
                                               const SetCellCallBack &callback)
{
    if(!sheet)
        return false;
    int iSize = model->rowCount(),
            jSize = model->columnCount();
    bool success = true;
    for(int i = 0; i < iSize && success; i++)
    {
        success &= writeCellVallue(sheet->querySubObject("Cells(int, int)",
                                                   i + 2 + shiftRow, 1 + shiftColumn),
                             model->headerData(i, Qt::Vertical));
        for(int j = 0; j < jSize && success; j++)
        {
            QAxObject *cell = sheet->querySubObject("Cells(int, int)", i + 2 + shiftRow,
                                                    j + 2 + shiftColumn);
            success &= callback(cell, model->index(i, j).data());
            cell->dynamicCall("BorderAround(xlContinuous)");
        }
    }
    for(int j = 0; j < jSize && success; j++)
    {
        success &= writeCellVallue(sheet->querySubObject("Cells(int, int)",
                                                   1 + shiftRow, j + 2 + shiftColumn),
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

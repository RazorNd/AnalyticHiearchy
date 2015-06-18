#include "analytichiearchyexcelsaver.h"
#include "../model/entringdatamodel.h"
#include "../model/criteriamatrix.h"
#include "../model/alternativesmatrix.h"
#include "../model/resultmatrix.h"
#include <QTextCodec>
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
    setModel(model);
    return write();
}

bool AnalyticHiearchyExcelSaver::write()
{
    if(_fileName.isEmpty())
        return false;
    if(!_model)
        return false;
    QAxObject excelApp("Excel.Application");
    QAxObject *workbook = excelApp.querySubObject("Workbooks")
            ->querySubObject("add()");

    processingWorkbook(workbook);


    excelApp.setProperty("Visible", true);
    return true;
}

void AnalyticHiearchyExcelSaver::exception(int, QString, QString, QString)
{

}


AnalyticHiearchyModel *AnalyticHiearchyExcelSaver::model() const
{
    return _model;
}

void AnalyticHiearchyExcelSaver::setModel(AnalyticHiearchyModel *model)
{
    _model = model;
}
QVariant AnalyticHiearchyExcelSaver::simpleDataCallback()
{
    return QVariant();
}

QVariant AnalyticHiearchyExcelSaver::ratingDataCallback()
{
    return QVariant("#\" \"?/?");
}

QAxObject * AnalyticHiearchyExcelSaver::getSheet(QAxObject *workbook, int number)
{
    if(!workbook)
        return 0;
    QAxObject *sheets = workbook->querySubObject("Sheets");
    if(!sheets)
        return 0;
    QAxObject *sheet = sheets->querySubObject("Item(int)", number);

    if(sheet)
        return sheet;

    //TODO: лист должен помещаться в конец
    sheet = sheets->querySubObject("Add()",
                                   QVariant(0),
                                   sheets->querySubObject("Item(int)",
                                                          sheets->property("Count"))
                                   ->asVariant());
    return sheet;
}

bool AnalyticHiearchyExcelSaver::processingWorkbook(QAxObject *workbook)
{
    if(!workbook)
        return false;

//    QAxObject *styles = workbook->querySubObject("Styles");
//    if(!styles)
//        qDebug()<< "Styles not not created";
//    else
//    {
//        int count = styles->property("Count").toInt();
//        qDebug() << "Count of styles: " << count;
//        for(int i = 0; i < count; i++)
//        {
//            QAxObject *style = styles->querySubObject("Item(int)", i);
//            if(style)
//            {
//                qDebug() << style->property("Name");
//            }
//        }
//    }


    if(!savingEntringModel(getSheet(workbook, 1)))
    {
        //TODO: add error not saving Entring Model
        return false;        
    }
    if(!savingCriteriaModel(getSheet(workbook, 2)))
    {
        //TODO: add error not saving Criteria Model
        return false;
    }
    int criteriaCount = _model->criteriaCount();
    for(int i = 0; i < criteriaCount; i++)
    {
        if(!savingAlternativeModel(i, getSheet(workbook, 3 + i)))
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

bool AnalyticHiearchyExcelSaver::savingEntringModel(QAxObject *sheet)
{
    if(!sheet)
        return false;
    return sheet->setProperty("Name", "Входящие данные") &&
            writeSheet(sheet, _model->entringModel(), &simpleDataCallback);
}

bool AnalyticHiearchyExcelSaver::savingCriteriaModel(QAxObject *sheet)
{
    if(!sheet)
        return false;
    return sheet->setProperty("Name", "Критерии") &&
            writeSheet(sheet, _model->criteriaMatrix(), &ratingDataCallback);

}

bool AnalyticHiearchyExcelSaver::savingAlternativeModel(int criteria, QAxObject *sheet)
{
    if(!sheet)
        return false;
    return sheet->setProperty("Name", _model->criterion(criteria)) &&
            writeSheet(sheet, _model->alternativesMatrix(criteria), &ratingDataCallback);
}

bool AnalyticHiearchyExcelSaver::savingResultModel(QAxObject *sheet)
{
    if(!sheet)
        return false;
    return sheet->setProperty("Name", "Результат") &&
            writeSheet(sheet, _model->resultMatrix(), &ratingDataCallback);
}

bool AnalyticHiearchyExcelSaver::writeSheet(QAxObject *sheet,
                                            QAbstractItemModel *model,
                                            QVariant (AnalyticHiearchyExcelSaver::*callBackData)())
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
            success &= writeCellVallue(sheet->querySubObject("Cells(int, int)",
                                                             i + 2, j + 2),
                                       model->index(i, j).data(),
                                       (this->*callBackData)());
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

bool AnalyticHiearchyExcelSaver::writeCellVallue(QAxObject *cell,
                                                 const QVariant &value,
                                                 const QVariant &style)
{
    if(!cell)
        return false;    
    if(style.isValid())
    {
        QAxObject *cellStyle = cell->querySubObject("Style");
        if(cellStyle)
            cellStyle->setProperty("NumberFormat", style);
    }
    if(value.isValid())
        return cell->setProperty("Value", value);
    return true;
}

AnalyticHiearchyExcelSaver::AnalyticHiearchyExcelSaver(QObject *parent) : QObject(parent)
{
}



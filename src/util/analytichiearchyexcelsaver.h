#ifndef ANALYTICHIEARCHYEXCELSAVER_H
#define ANALYTICHIEARCHYEXCELSAVER_H

#include "../model/analytichiearchymodel.h"
#include <QObject>
#include <QAxObject>

class AnalyticHiearchyExcelSaver : public QObject
{
    Q_OBJECT

    QString _fileName;
    QString _format;
    AnalyticHiearchyModel *_model;

    QVariant simpleDataCallback();
    QVariant ratingDataCallback();

    bool processingWorkbook(QAxObject *workbook);
    bool savingEntringModel(QAxObject *sheet);
    bool savingCriteriaModel(QAxObject *sheet);
    bool savingAlternativeModel(int criteria, QAxObject *sheet);
    bool savingResultModel(QAxObject *sheet);

    bool writeSheet(QAxObject *sheet,
                    QAbstractItemModel *model,
                    QVariant (AnalyticHiearchyExcelSaver::*callBackData)());
    bool writeCellVallue(QAxObject *cell, const QVariant &value, const QVariant &style  = QVariant());
    QAxObject *getSheet(QAxObject *workbook, int number);
public:
    AnalyticHiearchyExcelSaver(QObject *parent = 0);

    QString fileName() const;
    void setFileName(const QString &fileName);

    QString format() const;
    void setFormat(const QString &format);


    AnalyticHiearchyModel *model() const;
    void setModel(AnalyticHiearchyModel *model);

    bool write(AnalyticHiearchyModel *model);
    bool write();

private slots:
    void exception(int,QString,QString,QString);

public slots:
};

#endif // ANALYTICHIEARCHYEXCELSAVER_H

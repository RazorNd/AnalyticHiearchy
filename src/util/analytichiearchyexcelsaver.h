#ifndef ANALYTICHIEARCHYEXCELSAVER_H
#define ANALYTICHIEARCHYEXCELSAVER_H

#include "../model/alternativesmatrix.h"
#include <QObject>
#include <QAxObject>

class AnalyticHiearchyExcelSaver : public QObject
{
    Q_OBJECT

    QString _fileName;
    QString _format;

    void writeSheet(QAxObject *sheet, QAbstractItemModel *model);
    void writeCell(QAxObject *cell, const QVariant &value);
public:
    AnalyticHiearchyExcelSaver(QObject *parent = 0);

    QString fileName() const;
    void setFileName(const QString &fileName);

    QString format() const;
    void setFormat(const QString &format);

    bool write(AnalyticHiearchyModel *model);

signals:
    void exception(int,QString,QString,QString);

public slots:
};

#endif // ANALYTICHIEARCHYEXCELSAVER_H

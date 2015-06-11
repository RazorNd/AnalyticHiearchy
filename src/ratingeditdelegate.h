#ifndef RATINGEDITDELEGATE_H
#define RATINGEDITDELEGATE_H

#include "rating.h"
#include <QStyledItemDelegate>
#include <QComboBox>

class RatingEditDelegate: public QStyledItemDelegate
{
    const RatingScale *_scale;
public:
    RatingEditDelegate(QObject *parent = 0);

    void setScale(const RatingScale *scale);
    const RatingScale *scale() const;

    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // RATINGEDITDELEGATE_H

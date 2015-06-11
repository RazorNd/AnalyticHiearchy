#ifndef CRITERIADIRECTIONDELIGATE_H
#define CRITERIADIRECTIONDELIGATE_H

#include <QStyledItemDelegate>

class CriteriaDirectionDeligate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CriteriaDirectionDeligate(QObject* parent);

    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // CRITERIADIRECTIONDELIGATE_H

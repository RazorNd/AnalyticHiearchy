#include "criteriadirectiondeligate.h"
#include <QComboBox>

CriteriaDirectionDeligate::CriteriaDirectionDeligate(QObject *parent):
    QStyledItemDelegate(parent)
{
}

QWidget *CriteriaDirectionDeligate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QComboBox *comboBox = new QComboBox(parent);
    comboBox->addItems(QStringList() << "min" << "max");
    return comboBox;
}

void CriteriaDirectionDeligate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
    if(index.data(Qt::EditRole).toBool())
        comboBox->setCurrentIndex(1);
    else
        comboBox->setCurrentIndex(0);
}

void CriteriaDirectionDeligate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
    bool val = comboBox->currentIndex();
    model->setData(index, val);
}

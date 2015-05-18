#include "abstractprioritizationmatrix.h"
#include "ratingeditdelegate.h"
#include <QDebug>

const RatingScale *RatingEditDelegate::scale() const
{
    return _scale;
}

RatingEditDelegate::RatingEditDelegate(QObject *parent):
    QStyledItemDelegate(parent), _scale(0)
{
}

void RatingEditDelegate::setScale(const RatingScale *scale)
{
    _scale = scale;
}
QWidget *RatingEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    QComboBox *comboBox = new QComboBox(parent);
    comboBox->addItems(_scale->allRating());
    return comboBox;
}

void RatingEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
    comboBox->setCurrentIndex(_scale->ratingToNumber(index.data(Qt::EditRole).toInt()));
}

void RatingEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
    model->setData(index, _scale->numberToRating(comboBox->currentIndex()));
}



QSize RatingEditDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const AbstractPrioritizationMatrix *model = qobject_cast<const AbstractPrioritizationMatrix*>(index.model());
    if(model->isRatingCell(index))
    {
        return QSize(55, 0);
    }
    return QStyledItemDelegate::sizeHint(option, index);
}

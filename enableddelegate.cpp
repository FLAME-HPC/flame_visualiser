#include "enableddelegate.h"
#include <QCheckBox>

EnabledDelegate::EnabledDelegate(QObject * parent)
    : QItemDelegate(parent)
{
}

QWidget *EnabledDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &/*index*/) const
{
    QCheckBox *editor = new QCheckBox(parent);
    return editor;
}

void EnabledDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    bool value = index.data().toBool();

    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    emit( checkBox->setChecked(value) );
}

void EnabledDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    bool value = checkBox->isChecked();

    model->setData(index, value, Qt::EditRole);
}

void EnabledDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}


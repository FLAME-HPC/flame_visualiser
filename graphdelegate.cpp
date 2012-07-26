/*!
 * \file graphdelegate.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of graph delegate
 */
#include <QComboBox>
#include "./graphdelegate.h"

GraphDelegate::GraphDelegate(GraphSettingsModel *gsm, int type, QObject *parent)
    : QItemDelegate(parent) {
    gsmodel = gsm;
    type_ = type;
}


QWidget *GraphDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &/*index*/) const {
    QComboBox *editor = new QComboBox(parent);
    if (type_ == 0) {
        for (int i = 0; i < gsmodel->getPlots().count(); i++) {
            QString text = QString("Graph %1").arg(i+1);
            editor->insertItem(i, text);
        }
    }
    if (type_ == 1) {
        editor->insertItem(0, "iteration");
        editor->insertItem(1, "time scale");
    }
    return editor;
}

void GraphDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    QString value = index.data().toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);

    if (QString::compare(value, "") == 0) {
        comboBox->setCurrentIndex(index.row());
    } else {
        for (int i = 0; i < comboBox->count(); i++) {
            if (comboBox->itemText(i) == value)
                comboBox->setCurrentIndex(i);
        }
    }
}

void GraphDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();

    model->setData(index, value, Qt::EditRole);
}

void GraphDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const {
    editor->setGeometry(option.rect);
}

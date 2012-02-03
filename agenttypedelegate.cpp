/*!
 * \file agenttypedelegate.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of agent type delegate
 */
#include <QComboBox>
#include <QDebug>
#include "./agenttypedelegate.h"

AgentTypeDelegate::AgentTypeDelegate(QList<AgentType> * ats, QObject * parent)
    : QItemDelegate(parent) {
    agentTypes = ats;
}

QWidget *AgentTypeDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &/*index*/) const {
    QComboBox *editor = new QComboBox(parent);
    for (int i = 0; i < agentTypes->count(); i++) {
        editor->insertItem(i, agentTypes->at(i).name);
        // qDebug() << "AgentTypeDelegate::createEditor "
        // << agentTypes->at(i).name;
    }
    // editor->insertItem(agentTypes->count(), "add...");
    return editor;
}

void AgentTypeDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    QString value = index.data().toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    for (int i = 0; i < comboBox->count(); i++) {
        if (comboBox->itemText(i) == value)
            comboBox->setCurrentIndex(i);
    }
}

void AgentTypeDelegate::setModelData(QWidget *editor,
        QAbstractItemModel *model, const QModelIndex &index) const {
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();

    model->setData(index, value, Qt::EditRole);
}

void AgentTypeDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const {
    editor->setGeometry(option.rect);
}

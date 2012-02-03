/*!
 * \file positiondelegate.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of position delegate
 */
#include <QtGui>
#include "./positiondelegate.h"
#include "./positiondialog.h"
#include "./position.h"

PositionDelegate::PositionDelegate(QList<AgentType> *ats,
        VisualSettingsModel *model, QObject *parent)
    : QItemDelegate(parent) {
    vsm = model;
    agentTypes = ats;
}

void PositionDelegate::paint(QPainter *painter,
        const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (qVariantCanConvert<Position>(index.data())) {
         Position position = qVariantValue<Position>(index.data());
        // painter->drawText(option.rect, mpre.toString());

         if (option.state & QStyle::State_Selected)
             painter->fillRect(option.rect, option.palette.highlight());

         /*starRating.paint(painter, option.rect, option.palette,
                          StarRating::ReadOnly);*/
         position.paint(painter, option.rect, option.palette,
                          Position::ReadOnly);
     } else {
         QItemDelegate::paint(painter, option, index);
     }
}


QWidget *PositionDelegate::createEditor(QWidget */*parent*/,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &index) const {
        // qDebug() << "PositionDelegate parent: " << windowParent;

        PositionDialog *editor =
                new PositionDialog(agentTypes, vsm, index.row());

        connect(editor, SIGNAL(accepted()), this, SLOT(commitAndCloseEditor()));
        connect(editor, SIGNAL(rejected()), this, SLOT(commitAndCloseEditor()));

        // editor->setParent(windowParent);
        editor->setModal(true);
        // editor->move(100, 100);
        // editor->setWindowFlags(WShowModal);

        return editor;
}

void PositionDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    if (qVariantCanConvert<Position>(index.data())) {
        Position position = qVariantValue<Position>(index.data());
        PositionDialog *dialog = static_cast<PositionDialog*>(editor);

        dialog->setPosition(position);
    } else {
         QItemDelegate::setEditorData(editor, index);
    }
}

void PositionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    if (qVariantCanConvert<Position>(index.data())) {
         PositionDialog *dialog = static_cast<PositionDialog*>(editor);
         model->setData(index, qVariantFromValue(dialog->getPosition()));
    } else {
         QItemDelegate::setModelData(editor, model, index);
    }
}

/*void PositionDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}*/

void PositionDelegate::commitAndCloseEditor() {
    PositionDialog *editor = qobject_cast<PositionDialog *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

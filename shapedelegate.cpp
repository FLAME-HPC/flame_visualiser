/*!
 * \file shapedelegate.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of shape delegate
 */
#include <QtGui>
#include "./shapedelegate.h"
#include "./shape.h"
#include "./shapedialog.h"

ShapeDelegate::ShapeDelegate(QList<AgentType> *ats,
        VisualSettingsModel *vsm, QObject *parent)
    : QItemDelegate(parent) {
    vsmodel = vsm;
    agentTypes = ats;
}

void ShapeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const {
    if (qVariantCanConvert<Shape>(index.data())) {
         Shape shape = qVariantValue<Shape>(index.data());
        // painter->drawText(option.rect, mpre.toString());

         if (option.state & QStyle::State_Selected)
             painter->fillRect(option.rect, option.palette.highlight());

         /*starRating.paint(painter, option.rect, option.palette,
                          StarRating::ReadOnly);*/
         shape.paint(painter, option.rect, option.palette,
                          Shape::ReadOnly);
     } else {
         QItemDelegate::paint(painter, option, index);
     }
}


QWidget *ShapeDelegate::createEditor(QWidget */*parent*/,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &index) const {
        // qDebug() << "ShapeDelegate parent: " << windowParent;

        ShapeDialog *editor = new ShapeDialog(agentTypes, vsmodel, index.row());

        connect(editor, SIGNAL(okButton()),
                this, SLOT(commitAndCloseEditor()));
        connect(editor, SIGNAL(cancelButton()),
                this, SLOT(rejectAndCloseEditor()));

        editor->setModal(true);

        return editor;
}

void ShapeDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    if (qVariantCanConvert<Shape>(index.data())) {
        Shape shape = qVariantValue<Shape>(index.data());
        ShapeDialog *dialog = static_cast<ShapeDialog*>(editor);

        dialog->setShape(shape);
    } else {
         QItemDelegate::setEditorData(editor, index);
    }
}

void ShapeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    if (qVariantCanConvert<Shape>(index.data())) {
         ShapeDialog *dialog = static_cast<ShapeDialog*>(editor);
         model->setData(index, qVariantFromValue(dialog->getShape()));
    } else {
         QItemDelegate::setModelData(editor, model, index);
    }
}

/*void ShapeDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}*/

void ShapeDelegate::commitAndCloseEditor() {
    ShapeDialog *editor = qobject_cast<ShapeDialog *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

void ShapeDelegate::rejectAndCloseEditor() {
    ShapeDialog *editor = qobject_cast<ShapeDialog *>(sender());
    emit closeEditor(editor);
}


#include "shapedelegate.h"
#include "shape.h"
#include "shapedialog.h"
#include <QtGui>

ShapeDelegate::ShapeDelegate(QList<AgentType> *ats, VisualSettingsModel *vsm, QObject *parent)
    : QItemDelegate(parent)
{
    vsmodel = vsm;
    agentTypes = ats;
}

void ShapeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
 {
    if (qVariantCanConvert<Shape>(index.data()))
    {
         Shape shape = qVariantValue<Shape>(index.data());
        //painter->drawText(option.rect, mpre.toString());

         if (option.state & QStyle::State_Selected)
             painter->fillRect(option.rect, option.palette.highlight());

         /*starRating.paint(painter, option.rect, option.palette,
                          StarRating::ReadOnly);*/
         shape.paint(painter, option.rect, option.palette,
                          Shape::ReadOnly);
     }
     else
     {
         QItemDelegate::paint(painter, option, index);
     }
 }


QWidget *ShapeDelegate::createEditor(QWidget */*parent*/,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &index) const
{
        //qDebug() << "ShapeDelegate parent: " << windowParent;

        ShapeDialog *editor = new ShapeDialog(agentTypes, vsmodel, index.row());

        connect(editor, SIGNAL(accepted()), this, SLOT(commitAndCloseEditor()));
        connect(editor, SIGNAL(rejected()), this, SLOT(commitAndCloseEditor()));

        //editor->setParent(windowParent);
        editor->setModal(true);
        //editor->move(100, 100);
        //editor->setWindowFlags(WShowModal);

        return editor;
}

void ShapeDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    if (qVariantCanConvert<Shape>(index.data()))
    {
        Shape shape = qVariantValue<Shape>(index.data());
        ShapeDialog *dialog = static_cast<ShapeDialog*>(editor);

        dialog->setShape(shape);
    }
    else
    {
         QItemDelegate::setEditorData(editor, index);
    }
}

void ShapeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if (qVariantCanConvert<Shape>(index.data()))
    {
         ShapeDialog *dialog = static_cast<ShapeDialog*>(editor);
         model->setData(index, qVariantFromValue(dialog->getShape()));
    }
    else
    {
         QItemDelegate::setModelData(editor, model, index);
    }
}

/*void ShapeDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}*/

void ShapeDelegate::commitAndCloseEditor()
{
    ShapeDialog *editor = qobject_cast<ShapeDialog *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}


#include "conditiondelegate.h"
#include "QtGui"
#include "condition.h"
#include "conditiondialog.h"

ConditionDelegate::ConditionDelegate(QList<AgentType> *ats, VisualSettingsModel *model, QObject *parent)
    : QItemDelegate(parent)
{
    vsm = model;
    agentTypes = ats;
    isVisual = true;
}

ConditionDelegate::ConditionDelegate(QList<AgentType> *ats, GraphSettingsModel *model, QObject *parent)
    : QItemDelegate(parent)
{
    gsm = model;
    agentTypes = ats;
    isVisual = false;
}


void ConditionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
 {
    if (qVariantCanConvert<Condition>(index.data()))
    {
         Condition condition = qVariantValue<Condition>(index.data());
        //painter->drawText(option.rect, mpre.toString());

         if (option.state & QStyle::State_Selected)
             painter->fillRect(option.rect, option.palette.highlight());

         /*starRating.paint(painter, option.rect, option.palette,
                          StarRating::ReadOnly);*/
         condition.paint(painter, option.rect, option.palette,
                          Condition::ReadOnly);
     }
     else
     {
         QItemDelegate::paint(painter, option, index);
     }
 }


QWidget *ConditionDelegate::createEditor(QWidget */*parent*/,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &index) const
{
    QString agentType;

    // Todo Use flag instead of relying on pointers being set (check pointers too!)

    /* If visualsettingmodel */
    if(isVisual)
    {
        agentType = vsm->getRule(index.row())->agentType();
    }
    else
    /* If graphsettingmodel */
    {
        agentType = gsm->getPlot(index.row())->getYaxis();
    }

    ConditionDialog *editor = new ConditionDialog(agentTypes, agentType, index.row());

    connect(editor, SIGNAL(accepted()), this, SLOT(commitAndCloseEditor()));
    connect(editor, SIGNAL(rejected()), this, SLOT(commitAndCloseEditor()));

    //editor->setParent(windowParent);
    editor->setModal(true);
    //editor->move(100, 100);
    //editor->setWindowFlags(WShowModal);

    return editor;
}

void ConditionDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    if (qVariantCanConvert<Condition>(index.data()))
    {
        Condition condition = qVariantValue<Condition>(index.data());
        ConditionDialog *dialog = static_cast<ConditionDialog*>(editor);

        dialog->setCondition(condition);
    }
    else
    {
         QItemDelegate::setEditorData(editor, index);
    }
}

void ConditionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if (qVariantCanConvert<Condition>(index.data()))
    {
         ConditionDialog *dialog = static_cast<ConditionDialog*>(editor);
         model->setData(index, qVariantFromValue(dialog->getCondition()));
    }
    else
    {
         QItemDelegate::setModelData(editor, model, index);
    }
}

/*void conditionDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}*/

void ConditionDelegate::commitAndCloseEditor()
{
    ConditionDialog *editor = qobject_cast<ConditionDialog *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

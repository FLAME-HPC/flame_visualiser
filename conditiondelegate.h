#ifndef CONDITIONDELEGATE_H
#define CONDITIONDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QList>
#include "agenttype.h"
#include "visualsettingsmodel.h"
#include "graphsettingsmodel.h"

class ConditionDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ConditionDelegate(QList<AgentType> * ats = 0, VisualSettingsModel * model = 0, QObject *parent = 0);
    ConditionDelegate(QList<AgentType> * ats = 0, GraphSettingsModel * model = 0, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                    const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

private slots:
    void commitAndCloseEditor();

private:
    QList<AgentType> * agentTypes;
    VisualSettingsModel * vsm;
    GraphSettingsModel * gsm;
    bool isVisual;
};

#endif // CONDITIONDELEGATE_H

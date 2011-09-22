#ifndef POSITIONDELEGATE_H
#define POSITIONDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QList>
#include "agenttype.h"
#include "visualsettingsmodel.h"

class PositionDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    PositionDelegate(QList<AgentType> * ats = 0, VisualSettingsModel * model = 0, QObject *parent = 0);

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
};

#endif // POSITIONDELEGATE_H

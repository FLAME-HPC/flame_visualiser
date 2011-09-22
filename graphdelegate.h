#ifndef GRAPHDELEGATE_H
#define GRAPHDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include "graphsettingsmodel.h"

class GraphDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    GraphDelegate(GraphSettingsModel * gsm = 0, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    GraphSettingsModel * gsmodel;
};

#endif // GRAPHDELEGATE_H

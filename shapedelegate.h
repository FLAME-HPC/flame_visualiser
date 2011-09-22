#ifndef SHAPEDELEGATE_H
#define SHAPEDELEGATE_H

#include "visualsettingsmodel.h"
#include "visualsettingsitem.h"
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>

class ShapeDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ShapeDelegate(VisualSettingsModel * vsm = 0, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

private slots:
    void commitAndCloseEditor();

private:
    VisualSettingsModel * vsmodel;
};

#endif // SHAPEDELEGATE_H

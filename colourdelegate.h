#ifndef COLOURDELEGATE_H
#define COLOURDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>

class ColourDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ColourDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
    /*QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;*/
};

#endif // COLOURDELEGATE_H

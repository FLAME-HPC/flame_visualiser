/*!
 * \file colourdelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for colour delegate
 */
#ifndef COLOURDELEGATE_H_
#define COLOURDELEGATE_H_

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>

class ColourDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    explicit ColourDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
    /*QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;*/
};

#endif  // COLOURDELEGATE_H_

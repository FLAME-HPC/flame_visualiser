/*!
 * \file graphdelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for graph delegate
 */
#ifndef GRAPHDELEGATE_H_
#define GRAPHDELEGATE_H_

#include <QItemDelegate>
#include <QModelIndex>
#include "./graphsettingsmodel.h"

class GraphDelegate : public QItemDelegate {
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

#endif  // GRAPHDELEGATE_H_

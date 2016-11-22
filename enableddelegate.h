/*!
 * \file enableddelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for enabled delegate
 */
#ifndef ENABLEDDELEGATE_H_
#define ENABLEDDELEGATE_H_

#include <QItemDelegate>
#include <QModelIndex>

class EnabledDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    explicit EnabledDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif  // ENABLEDDELEGATE_H_

/*!
 * \file shapedelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for shape delegate
 */
#ifndef SHAPEDELEGATE_H_
#define SHAPEDELEGATE_H_

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include "./visualsettingsmodel.h"
#include "./visualsettingsitem.h"
#include "./agenttype.h"

class ShapeDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    ShapeDelegate(QList<AgentType> * ats = 0,
            VisualSettingsModel * vsm = 0, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

  private slots:
    void commitAndCloseEditor();
    void rejectAndCloseEditor();

  private:
    VisualSettingsModel * vsmodel;
    QList<AgentType> * agentTypes;
};

#endif  // SHAPEDELEGATE_H_

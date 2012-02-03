/*!
 * \file agenttypedelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for agent type delegate
 */
#ifndef AGENTTYPEDELEGATE_H_
#define AGENTTYPEDELEGATE_H_

#include <QItemDelegate>
#include <QModelIndex>
#include <QList>
#include "./visualsettingsmodel.h"
#include "./agenttype.h"

class AgentTypeDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    AgentTypeDelegate(QList<AgentType> * ats = 0, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

  private:
    QList<AgentType> * agentTypes;
};

#endif  // AGENTTYPEDELEGATE_H_

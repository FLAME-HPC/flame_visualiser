/*!
 * \file positiondelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for position delegate
 */
#ifndef POSITIONDELEGATE_H_
#define POSITIONDELEGATE_H_

#include <QItemDelegate>
#include <QModelIndex>
#include <QList>
#include "./agenttype.h"
#include "./visualsettingsmodel.h"

class PositionDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    PositionDelegate(QList<AgentType> * ats = 0,
            VisualSettingsModel * model = 0, QObject *parent = 0);

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

#endif  // POSITIONDELEGATE_H_

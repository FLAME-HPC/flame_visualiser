/*!
 * \file visualsettingsmodel.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for visual settings model
 */
#ifndef VISUALSETTINGSMODEL_H_
#define VISUALSETTINGSMODEL_H_

#include <QAbstractTableModel>
#include <QColor>
#include "./visualsettingsitem.h"
#include "./shape.h"

class VisualSettingsModel : public QAbstractTableModel {
    Q_OBJECT

  public:
    explicit VisualSettingsModel(QObject *parent = 0)
         : QAbstractTableModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;
     QVariant data(const QModelIndex &index, int role) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
     /* Functions to provide editing */
     Qt::ItemFlags flags(const QModelIndex &index) const;
     bool setData(const QModelIndex &index, const QVariant &value,
                  int role = Qt::EditRole);
     /* Adding and removing rows */
     bool insertRows(int position, int rows,
             const QModelIndex &index = QModelIndex());
     bool removeRows(int position, int rows,
             const QModelIndex &index = QModelIndex());

     QList<VisualSettingsItem *> getRules() const { return rules; }
     VisualSettingsItem * getRule(int row) const { return rules[row]; }

  signals:
     void ruleUpdated(int);

  public slots:
     void addRule();
     void addRule(QString agentType, Condition condition, Position x,
             Position y, Position z, Shape shape, QColor colour);
     void deleteRule(QModelIndex index);
     void deleteRules();
     void transitionUpdated(QModelIndex topLeft, QModelIndex bottomRight);

  private:
    QList<VisualSettingsItem *> rules;
};

#endif  // VISUALSETTINGSMODEL_H_

#ifndef VISUALSETTINGSMODEL_H
#define VISUALSETTINGSMODEL_H

#include <QAbstractTableModel>
#include "visualsettingsitem.h"
#include "shape.h"
#include <QColor>

class VisualSettingsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    VisualSettingsModel(QObject *parent = 0)
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
     bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
     bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

     QList<VisualSettingsItem *> getRules() const { return rules; }
     VisualSettingsItem * getRule(int row) const { return rules[row]; }

signals:

public slots:
     void addRule();
     void addRule(QString agentType, Condition condition, Position x, Position y, Position z, Shape shape, QColor colour);
     void deleteRule(QModelIndex index);
     void deleteRules();
     void transitionUpdated(QModelIndex topLeft, QModelIndex bottomRight);

private:
    QList<VisualSettingsItem *> rules;
};

#endif // VISUALSETTINGSMODEL_H

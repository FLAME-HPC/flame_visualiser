#ifndef GRAPHSETTINGSMODEL_H
#define GRAPHSETTINGSMODEL_H

#include <QAbstractTableModel>
#include "graphsettingsitem.h"
#include <QColor>
#include "agent.h"
#include "condition.h"

class GraphSettingsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    GraphSettingsModel(QList<Agent> *a = 0, QObject *parent = 0)
        : QAbstractTableModel(parent) { agents = a; }

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

     QList<GraphSettingsItem *> getPlots() const { return plots; }
     GraphSettingsItem * getPlot(int row) const { return plots[row]; }
     QList<GraphSettingsItem *> getPlotsInSameGraph(QModelIndex index);
     void switchEnabled(QModelIndex index);
     void setDisabled(QString graphName);

signals:
     void plotGraphChanged(GraphSettingsItem * gsi, QString oldGraph, QString newGraph);

public slots:
     void addPlot();
     void addPlot(QString g, QString x, QString y, Condition cond, QColor c, bool e);
     void deletePlot(QModelIndex index);
     void deletePlots();
     //void graph_window_closed();

private:
    QList<Agent> *agents;
    QList<GraphSettingsItem *> plots;
    //QList<GraphWidget *> graphs;
};

#endif // GRAPHSETTINGSMODEL_H

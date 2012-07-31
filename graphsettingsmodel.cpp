/*!
 * \file graphsettingsmodel.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of graph settings model
 */
#include <QtGui>
#include <QBool>
#include "./graphsettingsmodel.h"

int GraphSettingsModel::rowCount(const QModelIndex &/*parent*/) const {
     return plots.count();
}

int GraphSettingsModel::columnCount(const QModelIndex &/*parent*/) const {
     return 6;
}

QVariant GraphSettingsModel::data(const QModelIndex &index, int role) const {
     if (!index.isValid())
         return QVariant();

     if (index.row() >= plots.size())
         return QVariant();

     if (role == Qt::DisplayRole) {
        if (index.column() == 0) return plots.at(index.row())->getGraph();
        if (index.column() == 1) return plots.at(index.row())->getXaxis();
        if (index.column() == 2) return plots.at(index.row())->getYaxis();
        if (index.column() == 3)
            return qVariantFromValue(plots.at(index.row())->condition());
        if (index.column() == 4)
            return qVariantFromValue(plots.at(index.row())->getColour());
        return QVariant();
     } else if (role == Qt::CheckStateRole) {
         if (index.column() == 5) {
             if (plots.at(index.row())->getEnable())
                 return Qt::Checked;
             else
                 return Qt::Unchecked;
         }
     }

     return QVariant();
}

QVariant GraphSettingsModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal) {
         if (section == 0) return QString("Graph");
         else if (section == 1) return QString("X-Axis");
         else if (section == 2) return QString("Y-Axis");
         else if (section == 3) return QString("Condition");
         else if (section == 4) return QString("Colour");
         else if (section == 5) return QString("Enabled");
         else
             return QString("");
     } else {
         return QString("Row %1").arg(section);
     }
}

Qt::ItemFlags GraphSettingsModel::flags(const QModelIndex &index) const {
     if (!index.isValid())
         return Qt::ItemIsEnabled;

     if (index.column() == 4)
         return QAbstractItemModel::flags(index);
     else if (index.column() == 5) return (QAbstractItemModel::flags(index) |
             Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
     else
         return QAbstractItemModel::flags(index) | Qt::ItemIsEditable |
             Qt::ItemIsSelectable;
}

bool GraphSettingsModel::setData(const QModelIndex &index,
                               const QVariant &value, int role) {
     if (index.isValid() && role == Qt::EditRole) {
        if (index.column() == 0) {
            /* Only emit signal when the graph name has changed */
            if (QString::compare(plots[index.row()]->getGraph(),
                    value.toString()) != 0) {
                emit(plotGraphChanged(plots[index.row()],
                        plots[index.row()]->getGraph(), value.toString()));
                /* Make the enabled be the same as other plots in same graph */
                for (int i = 0; i < plots.count(); i++) {
                    if (i != index.row()) {
                        if (QString::compare(plots[i]->getGraph(),
                                value.toString()) == 0) {
                            plots.at(index.row())->setEnable(
                                    plots.at(i)->getEnable());
                            emit dataChanged(createIndex(index.row(), 5, 0),
                                    createIndex(index.row(), 5, 0));
                        }
                    }
                }
            }
            plots.at(index.row())->setGraph(value.toString());
        }
        if (index.column() == 1)
            plots.at(index.row())->setXaxis(value.toString());
        if (index.column() == 2)
            plots.at(index.row())->setYaxis(value.toString());
        if (index.column() == 3)
            plots.at(index.row())->setCondition(
                    qVariantValue<Condition>(value));
        if (index.column() == 4)
            plots.at(index.row())->setColour(qVariantValue<QColor>(value));
        if (index.column() == 5) {
            plots.at(index.row())->setEnable(qVariantValue<bool>(value));
        }
        emit dataChanged(index, index);
        return true;
     }
     return false;
}

bool GraphSettingsModel::insertRows(int position, int rows,
        const QModelIndex &/*parent*/) {
     beginInsertRows(QModelIndex(), position, position+rows-1);

     for (int row = position; row < position+rows; row++) {
         GraphSettingsItem * gsi = new GraphSettingsItem();
         // agentType, condition, x, y, z, shape, colour);
         // plots.append(vsi);
         plots.insert(row, gsi);
         // qDebug() << "insert row " << row;
     }

     endInsertRows();
     return true;
}

bool GraphSettingsModel::removeRows(int position, int rows,
        const QModelIndex &/*parent*/) {
     beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row = position; row < position+rows; row++) {
         plots.removeAt(row);
         // graphs.removeAt(row);
         // qDebug() << "remove row " << row;
     }

     endRemoveRows();
     return true;
}

void GraphSettingsModel::deletePlot(QModelIndex index) {
    this->removeRows(index.row(), 1);
}

void GraphSettingsModel::deletePlots() {
    while (plots.count() > 0) this->removeRows(0, 1);
}

void GraphSettingsModel::addPlot() {
    // this->insertRows(plots.count(), 1);

    beginInsertRows(QModelIndex(), plots.count(), plots.count());

    GraphSettingsItem * gsi = new GraphSettingsItem();
    gsi->setGraph(QString("Graph %1").arg(plots.count()+1));
    plots.insert(plots.count(), gsi);

    /*GraphWidget * gw = new GraphWidget(agents);
    gw->setGraph(gsi->getGraph());
    gw->addPlot(gsi);
    graphs.insert(graphs.count(), gw);*/

    endInsertRows();

    // return gw;
}

void GraphSettingsModel::addPlot(QString g, QString x, QString y,
        Condition cond, QColor c, bool e) {
    beginInsertRows(QModelIndex(), plots.count(), plots.count());

    GraphSettingsItem * gsi = new GraphSettingsItem(g, x, y, cond, c, e);
    plots.insert(plots.count(), gsi);

    /*GraphWidget * gw = new GraphWidget(agents);
    gw->setGraph(gsi->getGraph());
    gw->addPlot(gsi);
    graphs.insert(graphs.count(), gw);*/
    // connect(gw, window_closed(), this, graph_window_closed());

    endInsertRows();

    // return gw;
}

QList<GraphSettingsItem *> GraphSettingsModel::getPlotsInSameGraph(
        QModelIndex index) {
    QString graphName = plots.at(index.row())->getGraph();
    QList<GraphSettingsItem *> subplots;
    for (int i = 0; i < plots.count(); i++) {
        if (QString::compare(graphName, plots.at(i)->getGraph()) == 0) {
            subplots.append(plots.at(i));
        }
    }
    return subplots;
}

void GraphSettingsModel::switchEnabled(QModelIndex index) {
    QString graphName = plots.at(index.row())->getGraph();
    bool enabled = !(plots.at(index.row())->getEnable());

    for (int i = 0; i < plots.count(); i++) {
        if (QString::compare(graphName, plots.at(i)->getGraph()) == 0) {
            plots[i]->setEnable(enabled);
            emit dataChanged(createIndex(i, index.column(), 0),
                    createIndex(i, index.column(), 0));
        }
    }
}

void GraphSettingsModel::setDisabled(QString graphName) {
    for (int i = 0; i < plots.count(); i++) {
        if (QString::compare(graphName, plots.at(i)->getGraph()) == 0) {
            plots[i]->setEnable(false);
            emit dataChanged(createIndex(i, 5, 0), createIndex(i, 5, 0));
        }
    }
}

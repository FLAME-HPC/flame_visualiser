/*!
 * \file visualsettingsmodel.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of visual settings model
 */
#include <QtGui>
#include "./visualsettingsmodel.h"

int VisualSettingsModel::rowCount(const QModelIndex &/*parent*/) const {
     return rules.count();
}

int VisualSettingsModel::columnCount(const QModelIndex &/*parent*/) const {
     return 8;
}

QVariant VisualSettingsModel::data(const QModelIndex &index, int role) const {
     if (!index.isValid())
         return QVariant();

     if (index.row() >= rules.size())
         return QVariant();

     if (role == Qt::DisplayRole) {
        if (index.column() == 0)
            return rules.at(index.row())->agentType();
        if (index.column() == 1)
            return qVariantFromValue(rules.at(index.row())->condition());
        if (index.column() == 2)
            return qVariantFromValue(rules.at(index.row())->x());
        if (index.column() == 3)
            return qVariantFromValue(rules.at(index.row())->y());
        if (index.column() == 4)
            return qVariantFromValue(rules.at(index.row())->z());
        if (index.column() == 5)
            return qVariantFromValue(rules.at(index.row())->shape());
        if (index.column() == 6)
            return qVariantFromValue(rules.at(index.row())->colour());
        return
                QVariant();
     } else if (role == Qt::CheckStateRole) {
         if (index.column() == 7) {
             if (rules.at(index.row())->enabled())
                 return Qt::Checked;
             else
                 return Qt::Unchecked;
         }
     }
     return QVariant();
}

QVariant VisualSettingsModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal) {
         if (section == 0) return QString("Agent Type");
         else if (section == 1) return QString("Condition");
         else if (section == 2) return QString("X");
         else if (section == 3) return QString("Y");
         else if (section == 4) return QString("Z");
         else if (section == 5) return QString("Shape");
         else if (section == 6) return QString("Colour");
         else if (section == 7) return QString("Enabled");
         else
             return QString("");
     } else {
         return QString("Row %1").arg(section);
     }
}

Qt::ItemFlags VisualSettingsModel::flags(const QModelIndex &index) const {
     if (!index.isValid())
         return Qt::ItemIsEnabled;

     if (index.column() == 6) return QAbstractItemModel::flags(index);
     else if (index.column() == 7) return (QAbstractItemModel::flags(index) |
                  Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
     else
         return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool VisualSettingsModel::setData(const QModelIndex &index,
                               const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        if (index.column() == 0)
            rules.at(index.row())->setAgentType(value.toString());
        if (index.column() == 1)
            rules.at(index.row())->
                setCondition(qVariantValue<Condition>(value));
        if (index.column() == 2)
            rules.at(index.row())->setX(qVariantValue<Position>(value));
        if (index.column() == 3)
            rules.at(index.row())->setY(qVariantValue<Position>(value));
        if (index.column() == 4)
            rules.at(index.row())->setZ(qVariantValue<Position>(value));
        if (index.column() == 5)
            rules.at(index.row())->setShape(qVariantValue<Shape>(value));
        if (index.column() == 6)
            rules.at(index.row())->setColour(qVariantValue<QColor>(value));
        if (index.column() == 7)
            rules.at(index.row())->setEnabled(qVariantValue<bool>(value));
         emit dataChanged(index, index);
         emit ruleUpdated(index.row());
         return true;
     }
     return false;
}

bool VisualSettingsModel::insertRows(int position, int rows,
        const QModelIndex &/*parent*/) {
     beginInsertRows(QModelIndex(), position, position+rows-1);

     for (int row = position; row < position+rows; row++) {
         VisualSettingsItem * vsi = new VisualSettingsItem();
         // agentType, condition, x, y, z, shape, colour);
         // rules.append(vsi);
         rules.insert(row, vsi);
         // qDebug() << "insert row " << row;
     }

     endInsertRows();
     return true;
}

bool VisualSettingsModel::removeRows(int position, int rows,
        const QModelIndex &/*parent*/) {
     beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row = position; row < position+rows; row++) {
         rules.removeAt(row);
         // qDebug() << "remove row " << row;
     }

     endRemoveRows();
     return true;
}

void VisualSettingsModel::deleteRule(QModelIndex index) {
    this->removeRows(index.row(), 1);
}

void VisualSettingsModel::deleteRules() {
    while (rules.count() > 0) this->removeRows(0, 1);
}

void VisualSettingsModel::addRule() {
    this->insertRows(rules.count(), 1);
}

void VisualSettingsModel::addRule(QString agentType, Condition condition,
        Position x, Position y, Position z, Shape shape, QColor colour,
                                  bool enabled) {
    beginInsertRows(QModelIndex(), rules.count(), rules.count());

    VisualSettingsItem * vsi = new VisualSettingsItem(agentType, condition,
            x, y, z, shape, colour, enabled);
    rules.insert(rules.count(), vsi);

    endInsertRows();
}

void VisualSettingsModel::switchEnabled(QModelIndex index) {
    rules.at(index.row())->setEnabled(!(rules.at(index.row())->enabled()));
    emit dataChanged(createIndex(index.row(), index.column(), 0),
            createIndex(index.row(), index.column(), 0));
}

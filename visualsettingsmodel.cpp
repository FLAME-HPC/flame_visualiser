#include <QtGui>
#include "visualsettingsmodel.h"

int VisualSettingsModel::rowCount(const QModelIndex &/*parent*/) const
 {
     return rules.count();
 }

int VisualSettingsModel::columnCount(const QModelIndex &/*parent*/) const
 {
     return 7;
 }

QVariant VisualSettingsModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (index.row() >= rules.size())
         return QVariant();

     if (role == Qt::DisplayRole)
     {
        if(index.column() == 0) return rules.at(index.row())->agentType();
        if(index.column() == 1) return qVariantFromValue(rules.at(index.row())->condition());
        if(index.column() == 2) return qVariantFromValue(rules.at(index.row())->x());
        if(index.column() == 3) return qVariantFromValue(rules.at(index.row())->y());
        if(index.column() == 4) return qVariantFromValue(rules.at(index.row())->z());
        if(index.column() == 5) return qVariantFromValue(rules.at(index.row())->shape());
        if(index.column() == 6) return qVariantFromValue(rules.at(index.row())->colour());
        return QVariant();
     }
     return QVariant();
 }

QVariant VisualSettingsModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const
 {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal)
     {
         if(section == 0) return QString("Agent Type");
         else if(section == 1) return QString("Condition");
         else if(section == 2) return QString("X");
         else if(section == 3) return QString("Y");
         else if(section == 4) return QString("Z");
         else if(section == 5) return QString("Shape");
         else if(section == 6) return QString("Colour");
         else return QString("");
     }
     else
         return QString("Row %1").arg(section);
 }

Qt::ItemFlags VisualSettingsModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return Qt::ItemIsEnabled;

     if(index.column() == 6) return QAbstractItemModel::flags(index);
     else return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
 }

bool VisualSettingsModel::setData(const QModelIndex &index,
                               const QVariant &value, int role)
 {
     if (index.isValid() && role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            //emit( updateStateName(rules.at(index.row())->currentState()->name(), value.toString()) );
            rules.at(index.row())->setAgentType(value.toString());
        }
        if(index.column() == 1) rules.at(index.row())->setCondition(qVariantValue<Condition>(value));
        if(index.column() == 2) rules.at(index.row())->setX(qVariantValue<Position>(value));
        if(index.column() == 3) rules.at(index.row())->setY(qVariantValue<Position>(value));
        if(index.column() == 4) rules.at(index.row())->setZ(qVariantValue<Position>(value));
        if(index.column() == 5) rules.at(index.row())->setShape(qVariantValue<Shape>(value));
        if(index.column() == 6) rules.at(index.row())->setColour(qVariantValue<QColor>(value));
         emit dataChanged(index, index);
         return true;
     }
     return false;
 }

bool VisualSettingsModel::insertRows(int position, int rows, const QModelIndex &/*parent*/)
 {
     beginInsertRows(QModelIndex(), position, position+rows-1);

     for (int row = position; row < position+rows; row++)
     {
         VisualSettingsItem * vsi = new VisualSettingsItem();//agentType, condition, x, y, z, shape, colour);
         //rules.append(vsi);
         rules.insert(row, vsi);
         //qDebug() << "insert row " << row;
     }

     endInsertRows();
     return true;
 }

bool VisualSettingsModel::removeRows(int position, int rows, const QModelIndex &/*parent*/)
 {
     beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row = position; row < position+rows; row++)
     {
         rules.removeAt(row);
         //qDebug() << "remove row " << row;
     }

     endRemoveRows();
     return true;
 }

void VisualSettingsModel::deleteRule(QModelIndex index)
{
    this->removeRows(index.row(), 1);
}

void VisualSettingsModel::deleteRules()
{
    while(rules.count() > 0) this->removeRows(0, 1);
}

void VisualSettingsModel::addRule()
{
    this->insertRows(rules.count(), 1);
}

void VisualSettingsModel::addRule(QString agentType, Condition condition, Position x, Position y, Position z, Shape shape, QColor colour)
{
    beginInsertRows(QModelIndex(), rules.count(), rules.count());

    VisualSettingsItem * vsi = new VisualSettingsItem(agentType, condition, x, y, z, shape, colour);
    rules.insert(rules.count(), vsi);

    endInsertRows();
}

void VisualSettingsModel::transitionUpdated(QModelIndex /*topLeft*/, QModelIndex /*bottomRight*/)
{
    //ui->label->setText(QString("transitionChanged r: %1 c: %2").arg(topLeft.row(), topLeft.column()));

    //emit( updateScene() );

    // If mpre was updated
    /*if(topLeft.column() == 2)
    {
        //qDebug() << "mpre updated";

        // If another branch from the same state exists
        Transition * t = rules.at(topLeft.row());
        for(int i = 0; i < rules.size(); i++)
        {
            Transition * t2 = rules.at(i);
            if(t->currentState() == t2->currentState() && i != topLeft.row())
            {
                //qDebug() << "Found other branch: " << t2->name();
                t2->getMprePointer()->setNot(false);
                t2->getMprePointer()->setEnabled(t->mpre().enabled());
                t2->getMprePointer()->setName(t->mpre().name());
                if(t->mpre().op() == "==") t2->getMprePointer()->setOp("!=");
                if(t->mpre().op() == "!=") t2->getMprePointer()->setOp("==");
                if(t->mpre().op() == ">")  t2->getMprePointer()->setOp("<=");
                if(t->mpre().op() == "<")  t2->getMprePointer()->setOp(">=");
                if(t->mpre().op() == ">=") t2->getMprePointer()->setOp("<");
                if(t->mpre().op() == "<=") t2->getMprePointer()->setOp(">");
                t2->getMprePointer()->setValue(t->mpre().value());
            }
        }
    }*/
}

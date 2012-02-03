/*!
 * \file graphsettingsitem.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for graph settings item
 */
#ifndef GRAPHSETTINGSITEM_H_
#define GRAPHSETTINGSITEM_H_

#include <QString>
#include <QColor>
#include "./condition.h"

class GraphSettingsItem {
  public:
    GraphSettingsItem();
    GraphSettingsItem(QString g, QString x, QString y, Condition cond,
            QColor c, bool e);

    void setGraph(QString g) { graphString = g; }
    QString getGraph() { return graphString; }
    void setXaxis(QString x) { xaxisString = x; }
    QString getXaxis() { return xaxisString; }
    void setYaxis(QString y) { yaxisString = y; }
    QString getYaxis() { return yaxisString; }
    void setCondition(Condition c) { conditionCondition = c; }
    Condition condition() const { return conditionCondition; }
    void setColour(QColor c) { colourColour = c; }
    QColor getColour() { return colourColour; }
    void setEnable(bool e) { enable = e; }
    bool getEnable() { return enable; }

  private:
    QString graphString;
    QString xaxisString;
    QString yaxisString;
    Condition conditionCondition;
    QColor colourColour;
    bool enable;
};

#endif  // GRAPHSETTINGSITEM_H_

/*!
 * \file visualsettingsitem.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for visual settings item
 */
#ifndef VISUALSETTINGSITEM_H_
#define VISUALSETTINGSITEM_H_

#include <QString>
#include <QColor>
#include "./shape.h"
#include "./position.h"
#include "./condition.h"
#include "./ruleagent.h"

class VisualSettingsItem {
  public:
    VisualSettingsItem();
    VisualSettingsItem(QString agentType, Condition condition,
            Position x, Position y, Position z, Shape shape,
                       QColor colour, bool enabled);
    ~VisualSettingsItem() {}

    void setAgentType(QString n) { agentTypeString = n; }
    QString agentType() const { return agentTypeString; }
    void setCondition(Condition c) { conditionCondition = c; }
    Condition condition() const { return conditionCondition; }
    void setX(Position x) { xPosition = x; }
    Position x() const { return xPosition; }
    void setY(Position y) { yPosition = y; }
    Position y() const { return yPosition; }
    void setZ(Position z) { zPosition = z; }
    Position z() const { return zPosition; }
    void setShape(Shape s) { shapeShape = s; }
    Shape shape() const { return shapeShape; }
    void setColour(QColor c) { colourColor = c; }
    QColor colour() const { return colourColor; }
    void setEnabled(bool b) { boolEnabled = b; }
    bool enabled() const { return boolEnabled; }

    QList<RuleAgent *> agents;  /*!< The list of agents to draw */

  private:
    QString agentTypeString;
    // QString conditionString;
    Condition conditionCondition;
    // QString xString;
    // QString yString;
    // QString zString;
    Position xPosition;
    Position yPosition;
    Position zPosition;
    // QString shapeString;
    // QString colourString;
    Shape shapeShape;
    QColor colourColor;
    bool boolEnabled;
};

#endif  // VISUALSETTINGSITEM_H_

/*!
 * \file visualsettingsitem.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of visual settings item
 */
#include "./visualsettingsitem.h"

VisualSettingsItem::VisualSettingsItem() {
    colourColor = QColor(0, 0, 0, 255);
    boolEnabled = true;
}

VisualSettingsItem::VisualSettingsItem(QString agentType, Condition condition,
        Position x, Position y, Position z, Shape shape,
                                       QColor colour, bool enabled) {
    setAgentType(agentType);
    setCondition(condition);
    setX(x);
    setY(y);
    setZ(z);
    setShape(shape);
    setColour(colour);
    setEnabled(enabled);
}

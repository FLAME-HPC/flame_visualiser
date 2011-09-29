#include "visualsettingsitem.h"

VisualSettingsItem::VisualSettingsItem()
{
    colourColor = QColor(255,255,255,255);
}

VisualSettingsItem::VisualSettingsItem(QString agentType, Condition condition, Position x, Position y, Position z, Shape shape, QColor colour)
{
    setAgentType(agentType);
    setCondition(condition);
    setX(x);
    setY(y);
    setZ(z);
    setShape(shape);
    setColour(colour);
}

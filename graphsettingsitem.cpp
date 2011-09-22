#include "graphsettingsitem.h"

GraphSettingsItem::GraphSettingsItem()
{
    graphString = "";
    xaxisString = "iteration";
    yaxisString = "";
    conditionCondition = Condition();
    colourColour = QColor(0,0,0);
    enable = false;
}

GraphSettingsItem::GraphSettingsItem(QString g, QString x, QString y, Condition cond, QColor c, bool e)
{
    graphString = g;
    xaxisString = x;
    yaxisString = y;
    conditionCondition = cond;
    colourColour = c;
    enable = e;
}

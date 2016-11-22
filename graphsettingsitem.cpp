/*!
 * \file graphsettingsitem.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of graph settings item
 */
#include "./graphsettingsitem.h"

GraphSettingsItem::GraphSettingsItem() {
    graphString = "";
    xaxisString = "iteration";
    yaxisString = "";
    conditionCondition = Condition();
    colourColour = QColor(0, 0, 0);
    enable = false;
}

GraphSettingsItem::GraphSettingsItem(QString g, QString x, QString y,
        Condition cond, QColor c, bool e) {
    graphString = g;
    xaxisString = x;
    yaxisString = y;
    conditionCondition = cond;
    colourColour = c;
    enable = e;
}

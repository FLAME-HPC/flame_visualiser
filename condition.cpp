/*!
 * \file condition.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of condition
 */
#include "./condition.h"

Condition::Condition() {
    variable = "";
    op = "==";
    value = 0.0;
    enable = false;
}

QString Condition::getString() const {
    QString text;

    if (!enable) return "";

    text = variable;
    text.append(" ");
    text.append(op);
    text.append(" ");
    text.append(QString("%1").arg(value));

    return text;
}

void Condition::paint(QPainter *painter, const QRect &rect,
                       const QPalette &/*palette*/, EditMode /*mode*/) const {
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);

    if (enable) painter->drawText(rect, Qt::AlignCenter, getString());

    painter->restore();
}

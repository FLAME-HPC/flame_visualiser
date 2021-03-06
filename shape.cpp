/*!
 * \file shape.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of shape
 */
#include "./shape.h"

Shape::Shape() {
    shape = "sphere";
    shapes << "sphere" << "cube" << "point";
    dimension = 1.0;
    dimensionY = 1.0;
    dimensionZ = 1.0;
    dimensionVariable = "";
    dimensionVariableY = "";
    dimensionVariableZ = "";
    useVariable = false;
    useVariableY = false;
    useVariableZ = false;
    fromCentreX = false;
    fromCentreY = false;
    fromCentreZ = false;
}

void Shape::paint(QPainter *painter, const QRect &rect,
                       const QPalette &/*palette*/, EditMode /*mode*/) const {
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);

    QString text;
    if (QString::compare(shape, "") == 0) {
        text = "";
    } else {
        text.append(shape);
       /*text.append("(");
       if(useValue) text.append(QString("%1").arg(dimension));
       if(useVariable) text.append(dimensionVariable);
       if(QString::compare(shape,"cube") == 0)
       {
           text.append(",");
           if(useValueY) text.append(QString("%1").arg(dimensionY));
           if(useVariableY) text.append(dimensionVariableY);
           text.append(",");
           if(useValueZ) text.append(QString("%1").arg(dimensionZ));
           if(useVariableZ) text.append(dimensionVariableZ);
       }
       text.append(")");*/
    }

    painter->drawText(rect, Qt::AlignCenter, text);

    painter->restore();
}

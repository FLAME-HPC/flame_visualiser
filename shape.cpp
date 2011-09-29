#include "shape.h"

Shape::Shape()
{
    shape = "";
    shapes << "sphere" << "cube" << "point";
    dimension = 0.0;
    dimensionVariable = "";
    useValue = true;
    useVariable = false;
}

void Shape::paint(QPainter *painter, const QRect &rect,
                       const QPalette &/*palette*/, EditMode /*mode*/) const
{
   painter->save();

   painter->setRenderHint(QPainter::Antialiasing, true);

   QString text;
   if(QString::compare(shape, "") == 0) text = "";
   else
   {
       text.append(shape);
       text.append("(");
       if(useValue) text.append(QString("%1").arg(dimension));
       if(useVariable) text.append(dimensionVariable);
       text.append(")");
   }

   painter->drawText(rect,Qt::AlignCenter,text);

   painter->restore();
}

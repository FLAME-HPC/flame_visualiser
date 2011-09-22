#include "shape.h"

Shape::Shape()
{
    shape = "";
    shapes << "sphere" << "cube" << "point";
    dimension = 0.0;
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
       text.append(QString("%1").arg(dimension));
       text.append(")");
   }

   painter->drawText(rect,Qt::AlignCenter,text);

   painter->restore();
}

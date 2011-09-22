#include "position.h"

Position::Position()
{
    positionVariable = "";
    op = "";
    opVariable = "";
    opValue = 0.0;
    useOp = true;
}

void Position::paint(QPainter *painter, const QRect &rect,
                       const QPalette &/*palette*/, EditMode /*mode*/) const
{
   painter->save();

   painter->setRenderHint(QPainter::Antialiasing, true);

   QString text;
   text = positionVariable;
   if(opValue != 0.0)
   {
       //text.append(" ");
       //text.append(op);
       //text.append(" ");
       if(opValue > 0.0) text.append("+");
       text.append(QString("%1").arg(opValue));
   }

   painter->drawText(rect,Qt::AlignCenter,text);

   painter->restore();
}

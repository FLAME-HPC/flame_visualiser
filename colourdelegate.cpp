#include "colourdelegate.h"
#include <QColor>
#include <QPainter>

ColourDelegate::ColourDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

void ColourDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const
{
  if (qVariantCanConvert<QColor>(index.data()))
  {
       QColor colour = qVariantValue<QColor>(index.data());
      //painter->drawText(option.rect, mpre.toString());

       if (option.state & QStyle::State_Selected)
           painter->fillRect(option.rect, option.palette.highlight());

       painter->fillRect(option.rect, colour);
       int percent = colour.alphaF()*100.0;
       if(percent < 100)
        painter->drawText(option.rect, Qt::AlignCenter, QString("%1%").arg(percent));
   }
   else
   {
       QItemDelegate::paint(painter, option, index);
   }
}

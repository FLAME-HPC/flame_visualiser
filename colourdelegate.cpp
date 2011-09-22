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
       //painter->drawText(option.rect,"colour");
   }
   else
   {
       QItemDelegate::paint(painter, option, index);
   }
}

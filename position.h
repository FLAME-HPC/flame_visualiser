#ifndef POSITION_H
#define POSITION_H

#include <QMetaType>
#include <QPainter>

class Position
{
public:
    enum EditMode { Editable, ReadOnly };

    Position();

    void paint(QPainter *painter, const QRect &rect,
                    const QPalette &palette, EditMode mode) const;

    QString positionVariable;
    QString op;
    QString opVariable;
    double opValue;
    bool useOp;
    bool useVariable;
};

Q_DECLARE_METATYPE(Position)

#endif // POSITION_H

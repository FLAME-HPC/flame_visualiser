#ifndef CONDITION_H
#define CONDITION_H

#include <QMetaType>
#include <QPainter>

class Condition
{
public:
    enum EditMode { Editable, ReadOnly };

    Condition();
    QString getString() const;
    void paint(QPainter *painter, const QRect &rect,
                        const QPalette &palette, EditMode mode) const;

    QString variable;
    QString op;
    double value;
    bool enable;
};

Q_DECLARE_METATYPE(Condition)

#endif // CONDITION_H

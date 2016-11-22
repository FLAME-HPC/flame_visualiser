/*!
 * \file condition.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for condition
 */
#ifndef CONDITION_H_
#define CONDITION_H_

#include <QMetaType>
#include <QPainter>

class Condition {
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

#endif  // CONDITION_H_

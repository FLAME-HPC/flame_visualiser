/*!
 * \file position.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for position
 */
#ifndef POSITION_H_
#define POSITION_H_

#include <QMetaType>
#include <QPainter>

class Position {
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

#endif  // POSITION_H_

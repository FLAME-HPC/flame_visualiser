/*!
 * \file timescale.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for time scale
 */
#ifndef TIMESCALE_H_
#define TIMESCALE_H_

#include <QDebug>

class TimeScale {
  public:
    TimeScale();
    void calcTotalSeconds();
    void reset();
    QString calcTimeScale(int iteration);

    int millisecond;
    int second;
    int minute;
    int hour;
    int day;
    int totalseconds;
    bool enabled;
    bool displayInVisual;
    int lowestScale;
    double milliseconds;
    int seconds;
    int minutes;
    int hours;
    int days;
    QString timeString;
};

#endif  // TIMESCALE_H_

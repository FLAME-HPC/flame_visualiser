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
    TimeScale() {
        reset();
    }

    void calcTotalSeconds() {
        totalseconds = second;
        totalseconds += minute*60;
        totalseconds += hour*60*60;
        totalseconds += day*60*60*24;

        if (millisecond > 0) lowestScale = 0;
        else if (second > 0) lowestScale = 1;
        else if (minute > 0) lowestScale = 2;
        else if (hour   > 0) lowestScale = 3;
        else if (day    > 0) lowestScale = 4;
    }

    void reset() {
        lowestScale = 0;
        millisecond = 0;
        second = 0;
        minute = 0;
        hour = 0;
        day = 0;
        totalseconds = 0;
        enabled = false;
        displayInVisual = false;
    }

    int millisecond;
    int second;
    int minute;
    int hour;
    int day;
    int totalseconds;
    bool enabled;
    bool displayInVisual;
    int lowestScale;
};

#endif  // TIMESCALE_H_

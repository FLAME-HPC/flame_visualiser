#ifndef TIMESCALE_H
#define TIMESCALE_H

#include <QDebug>

class TimeScale
{
public:
    TimeScale()
    {
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

    void calcTotalSeconds()
    {
        totalseconds = second;
        totalseconds += minute*60;
        totalseconds += hour*60*60;
        totalseconds += day*60*60*24;

        if(millisecond > 0) lowestScale = 0;
        else if(second > 0) lowestScale = 1;
        else if(minute > 0) lowestScale = 2;
        else if(hour   > 0) lowestScale = 3;
        else if(day    > 0) lowestScale = 4;
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

#endif // TIMESCALE_H

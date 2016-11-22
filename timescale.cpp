/*!
 * \file timescale.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of time scale
 */
#include <math.h>
#include "./timescale.h"

TimeScale::TimeScale() {
    reset();
}

void TimeScale::calcTotalSeconds() {
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

void TimeScale::reset() {
    lowestScale = 0;
    millisecond = 0;
    second = 0;
    minute = 0;
    hour = 0;
    day = 0;
    totalseconds = 0;
    enabled = false;
    displayInVisual = false;
    timeString = "";
}

QString TimeScale::calcTimeScale(int iteration) {
    double tsecs;
    milliseconds = millisecond*iteration/1000.0;
    milliseconds = modf(milliseconds, &tsecs);
    int totalseconds2 = static_cast<int>(tsecs);
    totalseconds2 += totalseconds*iteration;

    QString ts;
    size_t BufSize = 1000;
    char buf[BufSize];
    days = static_cast<int>(totalseconds2/86400);
    hours = static_cast<int>((totalseconds2%86400)/3600);
    minutes = static_cast<int>(((totalseconds2%86400)%3600)/60);
    seconds = static_cast<int>(((totalseconds2%86400)%3600)%60);

    if (days > 0 || lowestScale == 4) {
        snprintf(buf, BufSize, "%d day", days);
        ts.append(QString().fromAscii(buf));
        if (days > 1)
            ts.append("s");
        else
            ts.append(" ");
        if (lowestScale < 4) ts.append(" ");
    }
    if (hours > 0 || lowestScale < 4) {
        snprintf(buf, BufSize, "%02d hrs", hours);
        ts.append(QString().fromAscii(buf));
        // if(hours > 1) ts.append("s"); else ts.append(" ");
        if (lowestScale < 3) ts.append(" ");
    }
    if (minutes > 0 || lowestScale < 3) {
        snprintf(buf, BufSize, "%02d mins", minutes);
        ts.append(QString().fromAscii(buf));
        // if(minutes > 1) ts.append("s"); else ts.append(" ");
        if (lowestScale < 2) ts.append(" ");
    }
    if (lowestScale == 1) {
        snprintf(buf, BufSize, "%02d s", seconds);
        ts.append(QString().fromAscii(buf));
    } else if (lowestScale == 0) {
        snprintf(buf, BufSize, "%02.3f s", seconds+milliseconds);
        ts.append(QString().fromAscii(buf));
    }

    timeString = ts;
    return ts;
}

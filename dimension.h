/*!
 * \file dimension.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for dimension
 */
#ifndef DIMENSION_H_
#define DIMENSION_H_

class Dimension {
  public:
    Dimension() {
        xmin = 0.0;
        xmax = 0.0;
        ymin = 0.0;
        ymax = 0.0;
        zmin = 0.0;
        zmax = 0.0;
        xminon = false;
        xmaxon = false;
        yminon = false;
        ymaxon = false;
        zminon = false;
        zmaxon = false;
    }

    double xmin;
    double xmax;
    double ymin;
    double ymax;
    double zmin;
    double zmax;
    bool xminon;
    bool xmaxon;
    bool yminon;
    bool ymaxon;
    bool zminon;
    bool zmaxon;
};

#endif  // DIMENSION_H_

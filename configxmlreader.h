/*!
 * \file configxmlreader.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for config XML reader
 */
#ifndef CONFIGXMLREADER_H_
#define CONFIGXMLREADER_H_

#include <QXmlStreamReader>
#include <QColor>
#include "./visualsettingsmodel.h"
#include "./graphsettingsmodel.h"
#include "./shape.h"
#include "./timescale.h"

class ConfigXMLReader : public QXmlStreamReader {
  public:
    ConfigXMLReader(VisualSettingsModel * vsm, GraphSettingsModel * gsm,
        QString * rD, TimeScale * ts, double * r,
        float * xr, float *yr, float *xm, float * ym, float * zm, int * delay);

    bool read(QIODevice * device);

  private:
    void readUnknownElement();
    void readConfig();
    void readResultsData();
    void readTimeScale();
    void readAnimation();
    void readVisual();
    void readRules();
    void readRule();
    Shape readShape();
    QColor readColour();
    Position readPosition();
    Condition readCondition();
    void readGraph();
    void readPlot();
    QString readLhs();
    double readRhs();
    QString readXaxis();
    QString readYaxis();

    VisualSettingsModel * vsmodel;
    GraphSettingsModel * gsmodel;
    QString * resultsData;
    double * ratio;
    TimeScale * timeScale;
    float * xrotate;
    float * yrotate;
    float * xmove;
    float * ymove;
    float * zmove;
    int * delayTime;
};

#endif  // CONFIGXMLREADER_H_

#ifndef CONFIGXMLREADER_H
#define CONFIGXMLREADER_H

#include <QXmlStreamReader>
#include "visualsettingsmodel.h"
#include "graphsettingsmodel.h"
#include "shape.h"
#include <QColor>
#include "timescale.h"

class ConfigXMLReader : public QXmlStreamReader
{
public:
    ConfigXMLReader(VisualSettingsModel * vsm, GraphSettingsModel * gsm, QString * rD, TimeScale * ts, double * r,
                    float * xr, float *yr, float *xm, float * ym, float * zm);

    bool read(QIODevice * device);

private:
    void readUnknownElement();
    void readConfig();
    void readResultsData();
    void readTimeScale();
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
};

#endif // CONFIGXMLREADER_H

#include "configxmlreader.h"
#include "shape.h"
#include "position.h"
#include <QColor>
#include <QDebug>

ConfigXMLReader::ConfigXMLReader(VisualSettingsModel *vsm, GraphSettingsModel *gsm, QString *rD, double *r)
{
    vsmodel = vsm;
    gsmodel = gsm;
    resultsData = rD;
    ratio = r;
}

bool ConfigXMLReader::read(QIODevice * device)
{
    setDevice(device);

    while (!atEnd())
    {
         readNext();

         if (isStartElement()) {
             if (name() == "flame_visualiser_config" && attributes().value("version") == "0.1")
                 readConfig();
             else
                 raiseError(QObject::tr("The file is not an flame_visualiser_config version 0.1 file."));
         }
     }

     return !error();
}

void ConfigXMLReader::readUnknownElement()
{
    Q_ASSERT(isStartElement());

     while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement())
             readUnknownElement();
     }

}

void ConfigXMLReader::readConfig()
{
    Q_ASSERT(isStartElement() && name() == "flame_visualiser_config");

     while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "resultsData")
                 readResultsData();
             else if (name() == "visual")
                 readVisual();
             else if (name() == "graph")
                 readGraph();
             else
                 readUnknownElement();
         }
     }
}

void ConfigXMLReader::readResultsData()
{
    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "directory")
                 *resultsData = readElementText();
             else
                 readUnknownElement();
         }
     }
}

void ConfigXMLReader::readVisual()
{
    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "ratio")
                 *ratio = readElementText().toDouble();
             else if (name() == "rules")
                 readRules();
             else
                 readUnknownElement();
         }
     }
}

void ConfigXMLReader::readRules()
{
    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "rule")
                 readRule();
             else
                 readUnknownElement();
         }
     }
}

void ConfigXMLReader::readRule()
{
    QString agentType;
    Condition condition;
    Position x;
    Position y;
    Position z;
    Shape shape;
    QColor colour;

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement())
         {
             if (name() == "agentType")
                 agentType = readElementText();
             else if (name() == "condition")
                 condition = readCondition();
             else if (name() == "x")
                 x  = readPosition();
             else if (name() == "y")
                 y = readPosition();
             else if (name() == "z")
                 z = readPosition();
             else if (name() == "shape")
                 shape = readShape();
             else if (name() == "colour")
                 colour = readColour();
             else
                 readUnknownElement();
         }
     }

    vsmodel->addRule(agentType, condition, x, y, z, shape, colour);
}

Shape ConfigXMLReader::readShape()
{
    Shape shape;

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "object")
                 shape.setShape(readElementText());
             else if (name() == "dimension")
                 shape.setDimension((readElementText().toDouble()));
             else
                 readUnknownElement();
         }
     }

    return shape;
}

Position ConfigXMLReader::readPosition()
{
    Position position;

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "variable")
                 position.positionVariable = readElementText();
             else if (name() == "offSet")
                 position.opValue = readElementText().toDouble();
             else
                 readUnknownElement();
         }
     }

    return position;
}

Condition ConfigXMLReader::readCondition()
{
    Condition condition;
    QString enable;

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "enable")
             {
                 enable = readElementText();
                 if(QString::compare(enable,"true") == 0) condition.enable = true;
                 else condition.enable = false;
             }
             else if (name() == "lhs")
                 condition.variable = readLhs();
             else if (name() == "operator")
                 condition.op = readElementText();
             else if (name() == "rhs")
                 condition.value = readRhs();
             else
                 readUnknownElement();
         }
     }

    return condition;
}

QColor ConfigXMLReader::readColour()
{
    QColor colour;

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "r")
                 colour.setRed(readElementText().toInt());
             else if (name() == "g")
                 colour.setGreen(readElementText().toInt());
             else if (name() == "b")
                 colour.setBlue(readElementText().toInt());
             else
                 readUnknownElement();
         }
     }

    return colour;
}

QString ConfigXMLReader::readLhs()
{
    QString agentType;

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "variable")
                 agentType = readElementText();
             else
                 readUnknownElement();
         }
     }

    return agentType;
}

double ConfigXMLReader::readRhs()
{
    double value = 0.0;

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "value")
                 value = readElementText().toDouble();
             else
                 readUnknownElement();
         }
     }

    return value;
}

void ConfigXMLReader::readGraph()
{
    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "plot")
                 readPlot();
             else
                 readUnknownElement();
         }
     }
}

void ConfigXMLReader::readPlot()
{
    QString graphNumber;
    QString xAxis;
    QString yAxis;
    Condition condition;
    QColor colour;

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement())
         {
             if (name() == "graphNumber")
                 graphNumber = readElementText();
             else if (name() == "xAxis")
                 xAxis = readXaxis();
             else if (name() == "yAxis")
                 yAxis  = readYaxis();
             else if (name() == "condition")
                 condition = readCondition();
             else if (name() == "colour")
                 colour = readColour();
             else
                 readUnknownElement();
         }
     }

    gsmodel->addPlot(graphNumber, xAxis, yAxis, condition, colour, false);
}

QString ConfigXMLReader::readXaxis()
{
    QString type;

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "type")
                 type = readElementText();
             else
                 readUnknownElement();
         }
     }

    return type;
}

QString ConfigXMLReader::readYaxis()
{
    QString agentType;

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "agentType")
                 agentType = readElementText();
             else
                 readUnknownElement();
         }
     }

    return agentType;
}

/*!
 * \file zeroxmlreader.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for zero XML reader
 */
#ifndef ZEROXMLREADER_H_
#define ZEROXMLREADER_H_

#include <QXmlStreamReader>
#include <QHash>
#include "./agent.h"
#include "./agenttype.h"
#include "./visualsettingsmodel.h"
#include "./dimension.h"

class ZeroXMLReader : public QXmlStreamReader {
  public:
    ZeroXMLReader(QList<Agent*> * a, QList<AgentType> * at,
            VisualSettingsModel * vsm, double r, Dimension * ad,
            QStringList * sat, QHash<QString, int> * atc,
            double xo, double yo, double zo);
    bool read(QIODevice * device);

  private:
    void readUnknownElement();
    void readEnvironmentXML();
    void readAgentXML();
    void readAgentsXML();
    void readZeroXML();
    QList<Agent*> * agents;
    QList<AgentType> * agentTypes;
    VisualSettingsModel * vsmodel;
    double ratio;
    Dimension * agentDimension;
    QStringList * stringAgentTypes;
    QHash<QString, int> * agentTypeCounts;
    double xoffset;
    double yoffset;
    double zoffset;
};

#endif  // ZEROXMLREADER_H_

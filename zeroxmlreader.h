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
#include "./agent.h"
#include "./agenttype.h"
#include "./visualsettingsmodel.h"
#include "./dimension.h"

class ZeroXMLReader : public QXmlStreamReader {
  public:
    ZeroXMLReader(QList<Agent> * a, QList<AgentType> * at,
            VisualSettingsModel * vsm, double * r, Dimension * ad);
    bool read(QIODevice * device, int flag);

  private:
    void readUnknownElement();
    void readEnvironmentXML(int flag);
    void readAgentXML(int flag);
    void readAgentsXML(int flag);
    void readZeroXML(int flag);
    void applyRulesToAgent(Agent * agent);
    QList<Agent> * agents;
    QList<AgentType> * agentTypes;
    VisualSettingsModel * vsmodel;
    double * ratio;
    Dimension * agentDimension;
};

#endif  // ZEROXMLREADER_H_

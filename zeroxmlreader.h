#ifndef ZEROXMLREADER_H
#define ZEROXMLREADER_H

#include <QXmlStreamReader>
#include "agent.h"
#include "agenttype.h"
#include "visualsettingsmodel.h"

class ZeroXMLReader : public QXmlStreamReader
{
public:
    ZeroXMLReader(QList<Agent> * a, QList<AgentType> * at);
    bool read(QIODevice * device, int flag);

private:
    void readUnknownElement();
    void readAgentXML(int flag);
    void readAgentsXML(int flag);
    void readZeroXML(int flag);
    QList<Agent> * agents;
    QList<AgentType> * agentTypes;
};

#endif // ZEROXMLREADER_H

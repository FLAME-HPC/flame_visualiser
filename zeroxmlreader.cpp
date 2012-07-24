/*!
 * \file zeroxmlreader.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of zero XML reader
 */
#include <QtGui>
#include "./zeroxmlreader.h"
#include "./visualsettingsitem.h"
#include "./ruleagent.h"

ZeroXMLReader::ZeroXMLReader(QList<Agent *> *a, QList<AgentType> *at,
        VisualSettingsModel *vsm, double r, Dimension * ad,
                             QStringList *sat, QHash<QString, int> *atc,
                             double xo, double yo, double zo) {
    agents = a;
    agentTypes = at;
    vsmodel = vsm;
    ratio = r;
    agentDimension = ad;
    stringAgentTypes = sat;
    agentTypeCounts = atc;
    xoffset = xo;
    yoffset = yo;
    zoffset = zo;

    agentDimension->xmin =  999999.9;
    agentDimension->xmax = -999999.9;
    agentDimension->ymin =  999999.9;
    agentDimension->ymax = -999999.9;
    agentDimension->zmin =  999999.9;
    agentDimension->zmax = -999999.9;
}

bool ZeroXMLReader::read(QIODevice * device) {
    setDevice(device);

    while (!atEnd()) {
    readNext();

    if (isStartElement()) {
        if (name() == "states")
            readZeroXML();
            else
            raiseError(QObject::tr(
                "The file does not contain the root tag 'states'"));
        }
    }

    // Populate rules with ruleagents
    for (int i = 0; i < vsmodel->rowCount(); i++) {
        vsmodel->getRule(i)->populate(agents);
        vsmodel->getRule(i)->copyAgentDrawDataToRuleAgentDrawData(agentDimension);
    }
    for (int i = 0; i < vsmodel->rowCount(); i++) {
        vsmodel->getRule(i)->applyOffset(xoffset, yoffset, zoffset);
        vsmodel->getRule(i)->applyRatio(ratio);
    }

    return !error();
}

void ZeroXMLReader::readUnknownElement() {
    Q_ASSERT(isStartElement());

     while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement())
             readUnknownElement();
     }
}

/* Read 0.xml ignore tags until an xagent if found and handle it.
*/
void ZeroXMLReader::readZeroXML() {
    Q_ASSERT(isStartElement() && name() == "states");

    // qDebug("readZeroXML()\n");

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "environment")
                 readEnvironmentXML();
             else if (name() == "agents")
                 readAgentsXML();
             else if (name() == "xagent")
                 readAgentXML();
             else
                 readUnknownElement();
         }
     }
}

void ZeroXMLReader::readEnvironmentXML() {
    // Create 'agent' with type environment
    Agent agent = Agent();
    int index = -1;
    agent.isEnvironment = true;
    agent.agentType = "environment";

    if (stringAgentTypes->contains("environment") == false) {
     // qDebug() << "new agent type found: environment";
     stringAgentTypes->append("environment");
     agentTypes->append(AgentType("environment"));
     index = agentTypes->count() - 1;
    }

    /* Make environment count one for iteration info */
    agentTypeCounts->insert("environment", 1);

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             agent.tags.append(name().toString());
             agent.values.append(readElementText());
             if (index != -1) {
                // qDebug() << "\t" << name().toString();
                (*agentTypes)[index].variables.append(name().toString());
             }
         }
     }

    //applyRulesToAgent(&agent);
}

void ZeroXMLReader::readAgentsXML() {
    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "xagent")
                 readAgentXML();
             else
                 readUnknownElement();
         }
     }
}

void ZeroXMLReader::readAgentXML() {
    Agent * agent = new Agent;
    QString agentname = "";
    QString elementName = "";
    int index = -1;

    while (!atEnd()) {
         readNext();

         if (isEndElement()) break;

         if (isStartElement()) {
             if (name() == "name") {
                 agentname = readElementText();
                 agent->agentType = agentname;
                 /* Increment agent counts for iteration info */
                 agentTypeCounts->insert(agentname,
                                        agentTypeCounts->value(agentname) + 1);
                 /* If agent type is unknown then add to agent list */
                 if (stringAgentTypes->contains(agentname) == false) {
                     // qDebug() << "new agent type found: " << agentname;
                     stringAgentTypes->append(agentname);
                     agentTypes->append(AgentType(agentname));
                     index = agentTypes->count() - 1;
                 } else { index = -1; }
             } else {
                 elementName = name().toString();
                 agent->tags.append(elementName);
                 agent->values.append(readElementText());
                 /* If agent is unknown then add variables to agent type */
                 if (index != -1)
                    // qDebug() << "\t" << elementName;
                    (*agentTypes)[index].variables.append(elementName);
             }
         }
     }

    // Old (keep for graphs)
    agents->append(agent);

    // New
    //applyRulesToAgent(agent);
}

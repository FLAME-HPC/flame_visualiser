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

ZeroXMLReader::ZeroXMLReader(QList<Agent> *a, QList<AgentType> *at,
        VisualSettingsModel *vsm, double * r, Dimension * ad,
                             QStringList *sat, QHash<QString, int> *atc) {
    agents = a;
    agentTypes = at;
    vsmodel = vsm;
    ratio = r;
    agentDimension = ad;
    stringAgentTypes = sat;
    agentTypeCounts = atc;

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

    applyRulesToAgent(&agent);
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
    Agent agent = Agent();
    QString agentname = "";
    QString elementName = "";
    int index = -1;

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "name") {
                 agentname = readElementText();
                 agent.agentType = agentname;
                 /* Increment agent counts for iteration info */
                 agentTypeCounts->insert(agentname, agentTypeCounts->value(agentname) + 1);
                 /* If agent type is unknown then add to agent list */
                 if (stringAgentTypes->contains(agentname) == false) {
                     // qDebug() << "new agent type found: " << agentname;
                     stringAgentTypes->append(agentname);
                     agentTypes->append(AgentType(agentname));
                     index = agentTypes->count() - 1;
                 } else {
                     index = -1;
                 }
             } else {
                 elementName = name().toString();
                 agent.tags.append(elementName);
                 agent.values.append(readElementText());
                 /* If agent is unknown then add variables to agent type */
                 if (index != -1) {
                    // qDebug() << "\t" << elementName;
                    (*agentTypes)[index].variables.append(elementName);
                 }
             }
         }
     }

    // Old (keep for graphs)
    agents->append(agent);

    // New
    applyRulesToAgent(&agent);
}

void ZeroXMLReader::applyRulesToAgent(Agent *agent) {
    for (int i = 0; i < vsmodel->rowCount(); i++) {
        VisualSettingsItem * vsi = vsmodel->getRule(i);

        if (agent->agentType == vsi->agentType()) {
            bool pass = true;

            // Check condition variable
            if (vsi->condition().enable) {
                pass = false;

                for (int k = 0; k < agent->tags.count(); k++) {
                    if (QString::compare(vsi->condition().variable,
                            agent->tags.at(k)) == 0) {
                        if (vsi->condition().op == "==" &&
                                agent->values.at(k).toDouble() ==
                                vsi->condition().value) pass = true;
                        else if (vsi->condition().op == "!=" &&
                                agent->values.at(k).toDouble() !=
                                vsi->condition().value) pass = true;
                        else if (vsi->condition().op == ">" &&
                                agent->values.at(k).toDouble() >
                                vsi->condition().value) pass = true;
                        else if (vsi->condition().op == "<" &&
                                agent->values.at(k).toDouble() <
                                vsi->condition().value) pass = true;
                        else if (vsi->condition().op == ">=" &&
                                agent->values.at(k).toDouble() >=
                                vsi->condition().value) pass = true;
                        else if (vsi->condition().op == "<=" &&
                                agent->values.at(k).toDouble() <=
                                vsi->condition().value) pass = true;
                    }
                }
            }

            if (pass) {
                agent->x = vsi->x().opValue;
                agent->y = vsi->y().opValue;
                agent->z = vsi->z().opValue;
                agent->shapeDimension = vsi->shape().getDimension();
                agent->shapeDimensionY = vsi->shape().getDimensionY();
                agent->shapeDimensionZ = vsi->shape().getDimensionZ();

                for (int k = 0; k < agent->tags.count(); k++) {
                    if (vsi->x().useVariable)
                        if (QString::compare(vsi->x().positionVariable,
                                agent->tags.at(k)) == 0)
                            agent->x += agent->values.at(k).toDouble();
                    if (vsi->y().useVariable)
                        if (QString::compare(vsi->y().positionVariable,
                                agent->tags.at(k)) == 0)
                            agent->y += agent->values.at(k).toDouble();
                    if (vsi->z().useVariable)
                        if (QString::compare(vsi->z().positionVariable,
                                agent->tags.at(k)) == 0)
                            agent->z += agent->values.at(k).toDouble();
                    if (vsi->shape().getUseVariable())
                        if (QString::compare(
                                vsi->shape().getDimensionVariable(),
                                agent->tags.at(k)) == 0)
                            agent->shapeDimension +=
                                    agent->values.at(k).toDouble();
                    if (vsi->shape().getUseVariableY())
                        if (QString::compare(
                                vsi->shape().getDimensionVariableY(),
                                agent->tags.at(k)) == 0)
                            agent->shapeDimensionY +=
                                    agent->values.at(k).toDouble();
                    if (vsi->shape().getUseVariableZ())
                        if (QString::compare(
                                vsi->shape().getDimensionVariableZ(),
                                agent->tags.at(k)) == 0)
                            agent->shapeDimensionZ +=
                                    agent->values.at(k).toDouble();
                }

                /* Calc agent scene dimension */
                if (agentDimension->xmin > agent->x)
                    agentDimension->xmin = agent->x;
                if (agentDimension->xmax < agent->x)
                    agentDimension->xmax = agent->x;
                if (agentDimension->ymin > agent->y)
                    agentDimension->ymin = agent->y;
                if (agentDimension->ymax < agent->y)
                    agentDimension->ymax = agent->y;
                if (agentDimension->zmin > agent->z)
                    agentDimension->zmin = agent->z;
                if (agentDimension->zmax < agent->z)
                    agentDimension->zmax = agent->z;

                agent->x *= *ratio;
                agent->y *= *ratio;
                agent->z *= *ratio;
                agent->shapeDimension *= *ratio;
                agent->shapeDimensionY *= *ratio;
                agent->shapeDimensionZ *= *ratio;
                vsi->agents.append(*agent);
            }
        }
    }
}

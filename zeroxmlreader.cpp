#include <QtGui>
#include "zeroxmlreader.h"

ZeroXMLReader::ZeroXMLReader(QList<Agent> *a, QList<AgentType> *at)
{
    agents = a;
    agentTypes = at;
}

bool ZeroXMLReader::read(QIODevice * device, int flag)
{
    setDevice(device);

    while (!atEnd())
    {
         readNext();

         if (isStartElement()) {
             if (name() == "states")
                 readZeroXML(flag);
             else
                 raiseError(QObject::tr("The file does not contain the root tag 'states'"));
         }
     }

     return !error();
}

void ZeroXMLReader::readUnknownElement()
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

/* Read 0.xml ignore tags until an xagent if found and handle it.
*/
void ZeroXMLReader::readZeroXML(int flag)
{
    Q_ASSERT(isStartElement() && name() == "states");

    //qDebug("readZeroXML()\n");

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "agents")
                 readAgentsXML(flag);
             else if (name() == "xagent")
                 readAgentXML(flag);
             else
                 readUnknownElement();
         }
     }
}

void ZeroXMLReader::readAgentsXML(int flag)
{
    //qDebug("readAgentsXML()\n");

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "xagent")
                 readAgentXML(flag);
             else
                 readUnknownElement();
         }
     }
}

void ZeroXMLReader::readAgentXML(int flag)
{
    Agent agent = Agent();
    QString agentname = "";
    QString elementName = "";
    QString elementText = "";
    int index = 0;

    //qDebug("readAgentXML()\n");

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if(flag == 0)
         {
             if (isStartElement())
             {
                 if (name() == "name")
                 {
                     agentname = readElementText();

                     /* Look for agent type in list */
                     index = -1;
                     for(int i = 0; i < agentTypes->count(); i++)
                     {
                         if(QString::compare(agentTypes->at(i).name, agentname) == 0) index = i;
                     }
                     if(index == -1)
                     {
                         agentTypes->append(AgentType(agentname));
                         index = agentTypes->count() - 1;
                     }
                     else index = -1;
                 }
                 else
                 {
                     if(index != -1)
                     {
                        elementName = name().toString();
                        (*agentTypes)[index].variables.append(elementName);
                     }

                     readUnknownElement();
                 }
             }
         }
         else if(flag == 1)
         {
             if (isStartElement())
             {
                 if (name() == "name")
                     agent.agentType = readElementText();
                 else
                 {
                     agent.tags.append(name().toString());
                     agent.values.append(readElementText());
                 }

                 /*else if (name() == "posx")
                     x = readElementText().toDouble();
                 else if (name() == "posy")
                     y = readElementText().toDouble();
                 else if (name() == "posz")
                     z = readElementText().toDouble();
                 else if (name() == "state")
                 {
                    state = readElementText().toInt();
                    if(state == 0) { r=1.0; g=0.0; b=0.0; }
                    else if(state == 1) { r=1.0; g=0.0; b=0.0; }
                    else if(state == 40) { r=0.0; g=1.0; b=0.0; }
                    else if(state == 41) { r=1.0; g=0.0; b=0.0; }
                    else if(state == 100) { r=1.0; g=1.0; b=0.0; }
                    else if(state == 300) { r=0.0; g=0.0; b=0.5; }
                    else if(state == 400) { r=1.0; g=0.5; b=0.0; }
                    else if(state == 401) { r=0.5; g=0.33; b=0.0; }
                    else if(state == 500) { r=1.0; g=0.5; b=0.5; }
                    else if(state == 501) { r=0.5; g=0.0; b=0.8; }
                    else if(state == 600) { r=0.8; g=0.0; b=0.5; }
                    else if(state == 601) { r=0.5; g=0.0; b=0.8; }
                    else if(state == 200) { r=1.0; g=0.0; b=0.0; }
                    else if(state == 201) { r=0.0; g=1.0; b=0.0; }
                    else if(state == 700) { r=1.0; g=0.0; b=1.0; }
                    else if(state == 701) { r=0.0; g=1.0; b=0.5; }
                    else if(state == 800) { r=1.0; g=0.3; b=1.0; }
                    else if(state == 801) { r=0.0; g=1.0; b=1.0; }
                    else if(state == 900) { r=0.0; g=0.3; b=1.0; }
                    else if(state == 901) { r=0.7; g=0.5; b=1.0; }
                 }
                 else
                     readUnknownElement();*/
             }
         }
     }

    //qDebug("%s %f %f %f", qPrintable(agentname), x, y, z);
    if(flag == 1) agents->append(agent);  //agents->append(Agent(agentname, x, y, z, r, g, b));
}

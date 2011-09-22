#ifndef AGENT_H
#define AGENT_H

#include <QStringList>

class Agent
{
public:
    Agent() {}
    Agent(QString agentType) { this->agentType = agentType; }

    QString agentType;
    QStringList tags;
    QStringList values;
};

#endif // AGENT_H

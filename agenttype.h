#ifndef AGENTTYPE_H
#define AGENTTYPE_H

#include <QString>
#include <QList>

class AgentType
{
public:
    AgentType() { agent = true; }
    AgentType(QString n) { name = n; agent = true; isEnvironment = false; }

    QString name;
    QList<QString> variables;
    bool agent;
    bool isEnvironment;
};

#endif // AGENTTYPE_H

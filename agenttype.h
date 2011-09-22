#ifndef AGENTTYPE_H
#define AGENTTYPE_H

#include <QString>
#include <QList>

class AgentType
{
public:
    AgentType();
    AgentType(QString n) { name = n; }

    QString name;
    QList<QString> variables;
};

#endif // AGENTTYPE_H

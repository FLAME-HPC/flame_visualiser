/*!
 * \file agenttype.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for agent type
 */
#ifndef AGENTTYPE_H_
#define AGENTTYPE_H_

#include <QString>
#include <QList>

class AgentType {
  public:
    AgentType() { agent = true; }
    explicit AgentType(QString n) {
        name = n;
        agent = true;
        isEnvironment = false;
    }

    QString name;
    QList<QString> variables;
    bool agent;
    bool isEnvironment;
};

#endif  // AGENTTYPE_H_

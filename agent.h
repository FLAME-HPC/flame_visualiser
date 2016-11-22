/*!
 * \file agent.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for agent
 */
#ifndef AGENT_H_
#define AGENT_H_

#include <QStringList>

class Agent {
  public:
    Agent() {
        agentType = "";
        x = 0.0;
        y = 0.0;
        z = 0.0;
        shapeDimension = 0.0;
        shapeDimensionY = 0.0;
        shapeDimensionZ = 0.0;
        conditionVariable = 0.0;
        isEnvironment = false;
        isPicked = false;
    }

    QString agentType;
    QStringList tags;
    QStringList values;
    double x;
    double y;
    double z;
    double shapeDimension;
    double shapeDimensionY;
    double shapeDimensionZ;
    double conditionVariable;
    bool isEnvironment;
    bool isPicked;
};

#endif  // AGENT_H_

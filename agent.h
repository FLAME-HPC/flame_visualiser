#ifndef AGENT_H
#define AGENT_H

#include <QStringList>

class Agent
{
public:
    Agent()
    {
        agentType = "";
        x = 0.0;
        y = 0.0;
        z = 0.0;
        shapeDimension = 0.0;
        shapeDimensionY = 0.0;
        shapeDimensionZ = 0.0;
        conditionVariable = 0.0;
        isEnvironment = false;
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
};

#endif // AGENT_H

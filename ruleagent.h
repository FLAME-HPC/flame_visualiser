/*!
 * \file ruleagent.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for rule agent
 */
#ifndef RULEAGENT_H
#define RULEAGENT_H

#include "./agent.h"

class RuleAgent {
  public:
    RuleAgent(Agent * a) {
        x = 0.0;
        y = 0.0;
        z = 0.0;
        shapeDimension = 0.0;
        shapeDimensionY = 0.0;
        shapeDimensionZ = 0.0;
        conditionVariable = 0.0;
        isEnvironment = false;
        isPicked = false;
        agent = a;
    }

    // Rule variables needed for drawing agent
    double x;
    double y;
    double z;
    double shapeDimension;
    double shapeDimensionY;
    double shapeDimensionZ;
    double conditionVariable;
    bool isEnvironment;
    bool isPicked;
    /*! \brief Pointer to original agent memory variables */
    Agent * agent;
};

#endif // RULEAGENT_H

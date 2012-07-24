/*!
 * \file visualsettingsitem.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of visual settings item
 */
#include "./visualsettingsitem.h"

VisualSettingsItem::VisualSettingsItem() {
    colourColor = QColor(128, 128, 128, 255);
    boolEnabled = true;
}

VisualSettingsItem::VisualSettingsItem(QString agentType, Condition condition,
        Position x, Position y, Position z, Shape shape,
                                       QColor colour, bool enabled) {
    setAgentType(agentType);
    setCondition(condition);
    setX(x);
    setY(y);
    setZ(z);
    setShape(shape);
    setColour(colour);
    setEnabled(enabled);
}

bool VisualSettingsItem::passAgentCondition(Agent *agent) {
    bool pass = true;
    // If condition is enabled
    if (conditionCondition.enable) {
        pass = false;
        // For each agent variable
        for (int k = 0; k < agent->tags.count(); k++) {
            // If the variable is used in the condition
            if (QString::compare(conditionCondition.variable,
                    agent->tags.at(k)) == 0) {
                // Check each possible operator and its outcome
                if (conditionCondition.op == "==" &&
                        agent->values.at(k).toDouble() ==
                        conditionCondition.value) pass = true;
                else if (conditionCondition.op == "!=" &&
                        agent->values.at(k).toDouble() !=
                        conditionCondition.value) pass = true;
                else if (conditionCondition.op == ">" &&
                        agent->values.at(k).toDouble() >
                        conditionCondition.value) pass = true;
                else if (conditionCondition.op == "<" &&
                        agent->values.at(k).toDouble() <
                        conditionCondition.value) pass = true;
                else if (conditionCondition.op == ">=" &&
                        agent->values.at(k).toDouble() >=
                        conditionCondition.value) pass = true;
                else if (conditionCondition.op == "<=" &&
                        agent->values.at(k).toDouble() <=
                        conditionCondition.value) pass = true;
            }
        }
    }
    return pass;
}

void VisualSettingsItem::populate(QList<Agent *> *a) {
    for (int j = 0; j < agents.size(); j++)
        // Free memory of ruleagent data
        delete agents.at(j);
    agents.clear();
    // If the rule is enabled
    if (boolEnabled) {
        // If the agent corresponds to the rule agent
        for (int i = 0; i < a->size(); i++) {
            if (a->at(i)->agentType == agentTypeString) {
                // If the agent passes any condition
                if (passAgentCondition(a->at(i))) {
                    // Change agent drawing variables for this rule
                    // Then add a copy of this agent to the agent list of
                    // the drawing rule.

                    // Create a new rule agent to handle drawing this agent
                    // under this rule
                    agents.append(new RuleAgent(a->at(i)));
                }
            }
        }
    }
}

void VisualSettingsItem::applyOffset(double xoffset, double yoffset, double zoffset) {
    for (int j = 0; j < agents.size(); j++) {
        agents.at(j)->x += xoffset;
        agents.at(j)->y += yoffset;
        agents.at(j)->z += zoffset;
    }
}

void VisualSettingsItem::applyRatio(double ratio) {
    for (int j = 0; j < agents.size(); j++) {
        agents.at(j)->x *= ratio;
        agents.at(j)->y *= ratio;
        agents.at(j)->z *= ratio;
        /* Point size does not need to be ratioed */
        if (shapeShape.getShape() != "point") {
            agents.at(j)->shapeDimension  *= ratio;
            agents.at(j)->shapeDimensionY *= ratio;
            agents.at(j)->shapeDimensionZ *= ratio;
        }
    }
}

void VisualSettingsItem::copyAgentDrawDataToRuleAgentDrawData(Dimension * agentDimension) {
    for (int j = 0; j < agents.size(); j++) {
        RuleAgent * ruleagent = agents.at(j);
        Agent * agent = ruleagent->agent;

        ruleagent->x = xPosition.opValue;
        ruleagent->y = yPosition.opValue;
        ruleagent->z = zPosition.opValue;
        ruleagent->shapeDimension  = shapeShape.getDimension();
        ruleagent->shapeDimensionY = shapeShape.getDimensionY();
        ruleagent->shapeDimensionZ = shapeShape.getDimensionZ();

        for (int k = 0; k < agent->tags.count(); k++) {
            if (xPosition.useVariable)
                if (QString::compare(xPosition.positionVariable,
                        agent->tags.at(k)) == 0)
                    ruleagent->x += agent->values.at(k).toDouble();
            if (yPosition.useVariable)
                if (QString::compare(yPosition.positionVariable,
                        agent->tags.at(k)) == 0)
                    ruleagent->y += agent->values.at(k).toDouble();
            if (zPosition.useVariable)
                if (QString::compare(zPosition.positionVariable,
                        agent->tags.at(k)) == 0)
                    ruleagent->z += agent->values.at(k).toDouble();
            if (shapeShape.getUseVariable())
                if (QString::compare(
                        shapeShape.getDimensionVariable(),
                        agent->tags.at(k)) == 0)
                    ruleagent->shapeDimension +=
                            agent->values.at(k).toDouble();
            if (shapeShape.getUseVariableY())
                if (QString::compare(
                        shapeShape.getDimensionVariableY(),
                        agent->tags.at(k)) == 0)
                    ruleagent->shapeDimensionY +=
                            agent->values.at(k).toDouble();
            if (shapeShape.getUseVariableZ())
                if (QString::compare(
                        shapeShape.getDimensionVariableZ(),
                        agent->tags.at(k)) == 0)
                    ruleagent->shapeDimensionZ +=
                            agent->values.at(k).toDouble();
        }

        if (shapeShape.getFromCentreX()) ruleagent->shapeDimension  *= 2.0;
        if (shapeShape.getFromCentreY()) ruleagent->shapeDimensionY *= 2.0;
        if (shapeShape.getFromCentreZ()) ruleagent->shapeDimensionZ *= 2.0;

        /* Calc agent scene dimension */
        if (agentDimension->xmin > ruleagent->x + ruleagent->shapeDimension)
            agentDimension->xmin = ruleagent->x + ruleagent->shapeDimension;
        if (agentDimension->xmax < ruleagent->x + ruleagent->shapeDimension)
            agentDimension->xmax = ruleagent->x + ruleagent->shapeDimension;
        if (agentDimension->ymin > ruleagent->y + ruleagent->shapeDimensionY)
            agentDimension->ymin = ruleagent->y + ruleagent->shapeDimensionY;
        if (agentDimension->ymax < ruleagent->y + ruleagent->shapeDimensionY)
            agentDimension->ymax = ruleagent->y + ruleagent->shapeDimensionY;
        if (agentDimension->zmin > ruleagent->z + ruleagent->shapeDimensionZ)
            agentDimension->zmin = ruleagent->z + ruleagent->shapeDimensionZ;
        if (agentDimension->zmax < ruleagent->z + ruleagent->shapeDimensionZ)
            agentDimension->zmax = ruleagent->z + ruleagent->shapeDimensionZ;
    }
}

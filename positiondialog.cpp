/*!
 * \file positiondialog.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of position dialog
 */
#include <QtGui>
#include <QHBoxLayout>
#include "./positiondialog.h"

PositionDialog::PositionDialog(QList<AgentType> *ats,
        VisualSettingsModel * model, int r, QWidget *parent)
    : QDialog(parent) {
    setupUi(this);

    vsm = model;
    agentTypes = ats;
    row = r;

    for (int i = 0; i < agentTypes->count(); i++) {
        if (QString::compare(agentTypes->at(i).name,
                vsm->getRule(row)->agentType()) == 0) {
            for (int j = 0; j < agentTypes->at(i).variables.count(); j++) {
               variableComboBox->insertItem(j,
                       agentTypes->at(i).variables.at(j));
            }
        }
    }
    valueSpinBox->setMaximum(9999.99);
    valueSpinBox->setMinimum(-9999.99);

    connect(checkBox_Variable, SIGNAL(clicked(bool)),
            variableComboBox, SLOT(setEnabled(bool)));
    connect(buttonBox, SIGNAL(accepted()), this, SIGNAL(okButton()));
    connect(buttonBox, SIGNAL(rejected()), this, SIGNAL(cancelButton()));
}

void PositionDialog::setPosition(Position p) {
    position = p;

    int index = 0;
    for (int i = 0; i < variableComboBox->count(); i++) {
        if (variableComboBox->itemText(i) == position.positionVariable)
            index = i;
    }
    variableComboBox->setCurrentIndex(index);

    valueSpinBox->setValue(position.opValue);

    checkBox_Variable->setChecked(position.useVariable);

    variableComboBox->setEnabled(position.useVariable);
}

Position PositionDialog::getPosition() {
    position.positionVariable = variableComboBox->currentText();
    position.opValue = valueSpinBox->value();
    position.useVariable = checkBox_Variable->checkState();
    return position;
}

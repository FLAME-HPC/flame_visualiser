#include "positiondialog.h"
#include <QtGui>
#include <QHBoxLayout>

PositionDialog::PositionDialog(QList<AgentType> *ats, VisualSettingsModel * model, int r, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    vsm = model;
    agentTypes = ats;
    row = r;

    for(int i = 0; i < agentTypes->count(); i++)
    {
        if(QString::compare(agentTypes->at(i).name, vsm->getRule(row)->agentType()) == 0)
        {
            for(int j = 0; j < agentTypes->at(i).variables.count(); j++)
            {
               variableComboBox->insertItem(j, agentTypes->at(i).variables.at(j));
            }
        }
    }
    valueSpinBox->setMaximum(9999.99);
    valueSpinBox->setMinimum(-9999.99);

    connect(this, SIGNAL(setVariableComboBox(int)), variableComboBox, SLOT(setCurrentIndex(int)));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void PositionDialog::setPosition(Position p)
{
    position = p;

    int index = 0;
    for(int i = 0; i < variableComboBox->count(); i++)
    {
        if(variableComboBox->itemText(i) == position.positionVariable)
            index = i;
    }
    emit ( setVariableComboBox(index) );

    valueSpinBox->setValue(position.opValue);
}

Position PositionDialog::getPosition()
{
    position.positionVariable = variableComboBox->currentText();
    position.opValue = valueSpinBox->value();
    return position;
}

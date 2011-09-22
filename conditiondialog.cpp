#include "conditiondialog.h"
#include <QtGui>

ConditionDialog::ConditionDialog(QList<AgentType> *ats, QString agentType, int r, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    agentTypes = ats;
    row = r;

    for(int i = 0; i < agentTypes->count(); i++)
    {
        if(QString::compare(agentTypes->at(i).name, agentType) == 0)
        {
            for(int j = 0; j < agentTypes->at(i).variables.count(); j++)
            {
               variableComboBox->insertItem(j, agentTypes->at(i).variables.at(j));
            }
        }
    }
    valueSpinBox->setMaximum(9999.99);
    valueSpinBox->setMinimum(-9999.99);
    operators << "==" << "!=" << ">" << "<" << ">=" << "<=";
    opComboBox->addItems(operators);

    connect(this, SIGNAL(setVariableComboBox(int)), variableComboBox, SLOT(setCurrentIndex(int)));
    connect(this, SIGNAL(setOpComboBox(int)), opComboBox, SLOT(setCurrentIndex(int)));
    connect(checkBox, SIGNAL(clicked(bool)), this, SLOT(updateEnable(bool)));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void ConditionDialog::updateEnable(bool c)
{
    variableComboBox->setEnabled(c);
    opComboBox->setEnabled(c);
    valueSpinBox->setEnabled(c);
}

void ConditionDialog::setCondition(Condition c)
{
    condition = c;

    emit( checkBox->setChecked(condition.enable) );
    updateEnable(condition.enable);

    int index = 0;
    for(int i = 0; i < variableComboBox->count(); i++)
    {
        if(variableComboBox->itemText(i) == condition.variable)
            index = i;
    }
    emit ( setVariableComboBox(index) );

    index = 0;
    for(int i = 0; i < opComboBox->count(); i++)
    {
        if(opComboBox->itemText(i) == condition.op)
            index = i;
    }
    emit ( setOpComboBox(index) );

    valueSpinBox->setValue(condition.value);
}

Condition ConditionDialog::getCondition()
{
    condition.enable = checkBox->isChecked();
    condition.variable = variableComboBox->currentText();
    condition.op = opComboBox->currentText();
    condition.value = valueSpinBox->value();
    return condition;
}

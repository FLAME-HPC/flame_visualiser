#include <QtGui>
#include <QHBoxLayout>
#include "shapedialog.h"

ShapeDialog::ShapeDialog(QList<AgentType> *ats, VisualSettingsModel *vsm, int r, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    vsmodel = vsm;
    agentTypes = ats;
    row = r;

    for(int i = 0; i < agentTypes->count(); i++)
    {
        if(QString::compare(agentTypes->at(i).name, vsm->getRule(row)->agentType()) == 0)
        {
            for(int j = 0; j < agentTypes->at(i).variables.count(); j++)
            {
                comboBox_Dimension->insertItem(j, agentTypes->at(i).variables.at(j));
            }
        }
    }
    doubleSpinBox_Dimension->setMaximum(9999.99);
    comboBox_Shape->addItems(shape.Shapes());



    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(checkBox_Value, SIGNAL(clicked(bool)), this, SLOT(valueClicked(bool)));
    connect(checkBox_Variable, SIGNAL(clicked(bool)), this, SLOT(variableClicked(bool)));
}

void ShapeDialog::setShape(Shape s)
{
    shape = s;

    int index = shape.Shapes().indexOf(shape.getShape());
    if(index == -1) index = 0;
    comboBox_Shape->setCurrentIndex(index);

    index = 0;
    for(int i = 0; i < comboBox_Dimension->count(); i++)
    {
        if(QString::compare(comboBox_Dimension->itemText(i),shape.getDimensionVariable()) == 0)
            index = i;
    }
    comboBox_Dimension->setCurrentIndex(index);

    doubleSpinBox_Dimension->setValue(shape.getDimension());

    if(shape.getUseValue())
    {
        checkBox_Value->setChecked(true);
        checkBox_Variable->setChecked(false);
        comboBox_Dimension->setEnabled(false);
    }
    if(shape.getUseVariable())
    {
        checkBox_Variable->setChecked(true);
        checkBox_Value->setChecked(false);
        doubleSpinBox_Dimension->setEnabled(false);
    }
}

Shape ShapeDialog::getShape()
{
    shape.setShape(comboBox_Shape->currentText());
    shape.setDimension(doubleSpinBox_Dimension->value());
    shape.setDimensionVariable(comboBox_Dimension->currentText());
    shape.setUseValue(checkBox_Value->isChecked());
    shape.setUseVariable(checkBox_Variable->isChecked());
    return shape;
}
void ShapeDialog::valueClicked(bool b)
{
    doubleSpinBox_Dimension->setEnabled(b);
    checkBox_Variable->setChecked(!b);
    comboBox_Dimension->setEnabled(!b);
}

void ShapeDialog::variableClicked(bool b)
{
    comboBox_Dimension->setEnabled(b);
    checkBox_Value->setChecked(!b);
    doubleSpinBox_Dimension->setEnabled(!b);
}

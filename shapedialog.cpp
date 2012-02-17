/*!
 * \file shapedialog.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of shape dialog
 */
#include <QtGui>
#include <QHBoxLayout>
#include "./shapedialog.h"

ShapeDialog::ShapeDialog(QList<AgentType> *ats,
        VisualSettingsModel *vsm, int r, QWidget *parent)
    : QDialog(parent) {
    setupUi(this);

    vsmodel = vsm;
    agentTypes = ats;
    row = r;

    for (int i = 0; i < agentTypes->count(); i++) {
        if (QString::compare(agentTypes->at(i).name,
                vsm->getRule(row)->agentType()) == 0) {
            for (int j = 0; j < agentTypes->at(i).variables.count(); j++) {
                comboBox_X->insertItem(j, agentTypes->at(i).variables.at(j));
                comboBox_Y->insertItem(j, agentTypes->at(i).variables.at(j));
                comboBox_Z->insertItem(j, agentTypes->at(i).variables.at(j));
            }
        }
    }
    doubleSpinBox_X->setMaximum(9999.99);
    doubleSpinBox_Y->setMaximum(9999.99);
    doubleSpinBox_Z->setMaximum(9999.99);
    comboBox_Shape->addItems(shape.Shapes());

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    connect(comboBox_Shape, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(shapeUpdated(QString)));

    connect(checkBox_X, SIGNAL(clicked(bool)),
            comboBox_X, SLOT(setEnabled(bool)));
    connect(checkBox_Y, SIGNAL(clicked(bool)),
            comboBox_Y, SLOT(setEnabled(bool)));
    connect(checkBox_Z, SIGNAL(clicked(bool)),
            comboBox_Z, SLOT(setEnabled(bool)));
}

void ShapeDialog::setShape(Shape s) {
    shape = s;

    int index = shape.Shapes().indexOf(shape.getShape());
    if (index == -1) index = 0;
    comboBox_Shape->setCurrentIndex(index);
    shapeUpdated(shape.getShape());

    index = 0;
    for (int i = 0; i < comboBox_X->count(); i++) {
        if (QString::compare(comboBox_X->itemText(i),
                shape.getDimensionVariable()) == 0) index = i;
    }
    comboBox_X->setCurrentIndex(index);
    index = 0;
    for (int i = 0; i < comboBox_Y->count(); i++) {
        if (QString::compare(comboBox_Y->itemText(i),
                shape.getDimensionVariableY()) == 0) index = i;
    }
    comboBox_Y->setCurrentIndex(index);
    index = 0;
    for (int i = 0; i < comboBox_Z->count(); i++) {
        if (QString::compare(comboBox_Z->itemText(i),
                shape.getDimensionVariableZ()) == 0) index = i;
    }
    comboBox_Z->setCurrentIndex(index);

    if (QString::compare(shape.getShape(), "sphere") == 0) {
        groupBox_Quality->setEnabled(true);
        spinBoxQuality->setValue(shape.getQuality());
    } else {
        spinBoxQuality->setValue(16);
        groupBox_Quality->setEnabled(false);
    }

    doubleSpinBox_X->setValue(shape.getDimension());
    doubleSpinBox_Y->setValue(shape.getDimensionY());
    doubleSpinBox_Z->setValue(shape.getDimensionZ());

    comboBox_X->setEnabled(shape.getUseVariable());
    comboBox_Y->setEnabled(shape.getUseVariableY());
    comboBox_Z->setEnabled(shape.getUseVariableZ());

    checkBox_X->setChecked(shape.getUseVariable());
    checkBox_Y->setChecked(shape.getUseVariableY());
    checkBox_Z->setChecked(shape.getUseVariableZ());

    checkBox_X_from_centre->setChecked(shape.getFromCentreX());
    checkBox_Y_from_centre->setChecked(shape.getFromCentreY());
    checkBox_Z_from_centre->setChecked(shape.getFromCentreZ());
}

Shape ShapeDialog::getShape() {
    shape.setShape(comboBox_Shape->currentText());

    shape.setDimension(doubleSpinBox_X->value());
    shape.setDimensionVariable(comboBox_X->currentText());
    shape.setUseVariable(checkBox_X->isChecked());
    shape.setFromCentreX(checkBox_X_from_centre->isChecked());

    shape.setDimensionY(doubleSpinBox_Y->value());
    shape.setDimensionVariableY(comboBox_Y->currentText());
    shape.setUseVariableY(checkBox_Y->isChecked());
    shape.setFromCentreY(checkBox_Y_from_centre->isChecked());

    shape.setDimensionZ(doubleSpinBox_Z->value());
    shape.setDimensionVariableZ(comboBox_Z->currentText());
    shape.setUseVariableZ(checkBox_Z->isChecked());
    shape.setFromCentreZ(checkBox_Z_from_centre->isChecked());

    shape.setQuality(spinBoxQuality->value());

    return shape;
}

void ShapeDialog::enableYZ(bool e) {
    checkBox_Y->setEnabled(e);
    checkBox_Z->setEnabled(e);
    doubleSpinBox_Y->setEnabled(e);
    doubleSpinBox_Z->setEnabled(e);
    checkBox_Y_from_centre->setEnabled(e);
    checkBox_Z_from_centre->setEnabled(e);
    if (!e || shape.getUseVariableY()) comboBox_Y->setEnabled(e);
    if (!e || shape.getUseVariableZ()) comboBox_Z->setEnabled(e);
    if (e) label_X->setText("X axis");
    else
        label_X->setText("XYZ axis");

    QPalette palet;
    if (e) palet.setColor(label_Y->foregroundRole(), QColor(0, 0, 0));
    else
        palet.setColor(label_Y->foregroundRole(), QColor(120, 120, 120));
    label_Y->setPalette(palet);
    label_Z->setPalette(palet);
}

void ShapeDialog::shapeUpdated(QString s) {
    if (QString::compare("cube", s) == 0) {
        enableYZ(true);
    } else {
        enableYZ(false);
    }

    if (QString::compare("sphere", s) == 0) {
        groupBox_Quality->setEnabled(true);
    } else {
        groupBox_Quality->setEnabled(false);
    }
}

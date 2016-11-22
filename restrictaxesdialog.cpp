/*!
 * \file restrictaxesdialog.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of restrict axes dialog
 */
#include <QDebug>
#include "./restrictaxesdialog.h"
#include "./ui_restrictaxesdialog.h"

RestrictAxesDialog::RestrictAxesDialog(Dimension * rd, Dimension * ad,
        Dimension * rad, double * r, QWidget *parent) :
    QDialog(parent), ui(new Ui::RestrictAxesDialog) {
    ui->setupUi(this);

    /* Initialise variables */
    restrictDimension = rd;
    agentDimension = ad;
    restrictAgentDimension = rad;
    ratio = r;
    xminBlock = false;
    xmaxBlock = false;
    yminBlock = false;
    ymaxBlock = false;
    zminBlock = false;
    zmaxBlock = false;
    firstDim = true;

    /* Set range of sliders */
    updatedAgentDimensions();

    /* Connect ui spin box changes to functions */
    connect(ui->doubleSpinBox_xMax, SIGNAL(valueChanged(double)),
            this, SLOT(updateXMax(double)));
    connect(ui->doubleSpinBox_xMin, SIGNAL(valueChanged(double)),
            this, SLOT(updateXMin(double)));
    connect(ui->doubleSpinBox_yMax, SIGNAL(valueChanged(double)),
            this, SLOT(updateYMax(double)));
    connect(ui->doubleSpinBox_yMin, SIGNAL(valueChanged(double)),
            this, SLOT(updateYMin(double)));
    connect(ui->doubleSpinBox_zMax, SIGNAL(valueChanged(double)),
            this, SLOT(updateZMax(double)));
    connect(ui->doubleSpinBox_zMin, SIGNAL(valueChanged(double)),
            this, SLOT(updateZMin(double)));
    /* Connect ui sliders changes to functions */
    connect(ui->horizontalSlider_xMin, SIGNAL(sliderMoved(int)),
            this, SLOT(updateXMinS(int)));
    connect(ui->horizontalSlider_xMax, SIGNAL(sliderMoved(int)),
            this, SLOT(updateXMaxS(int)));
    connect(ui->horizontalSlider_yMin, SIGNAL(sliderMoved(int)),
            this, SLOT(updateYMinS(int)));
    connect(ui->horizontalSlider_yMax, SIGNAL(sliderMoved(int)),
            this, SLOT(updateYMaxS(int)));
    connect(ui->horizontalSlider_zMin, SIGNAL(sliderMoved(int)),
            this, SLOT(updateZMinS(int)));
    connect(ui->horizontalSlider_zMax, SIGNAL(sliderMoved(int)),
            this, SLOT(updateZMaxS(int)));
    /* Connect accept reject buttons to close function */
    connect(this, SIGNAL(accepted()), this, SLOT(close()));
    connect(this, SIGNAL(rejected()), this, SLOT(close()));
}

RestrictAxesDialog::~RestrictAxesDialog() {
    delete ui;
}

/*!
 * \brief Calculate min and max dimensions of agents and apply to ui
 *
 * Calculate min and max dimensions of agents and update spin box and slider config.
 */
void RestrictAxesDialog::updatedAgentDimensions() {
    /* Get agent space dimensions and add buffer */
    xStartK = agentDimension->xmin*1.1;
    xEndK   = agentDimension->xmax*1.1;
    yStartK = agentDimension->ymin*1.1;
    yEndK   = agentDimension->ymax*1.1;
    zStartK = agentDimension->zmin*1.1;
    zEndK   = agentDimension->zmax*1.1;

    /* If first run then make values the extreme
       else only if they are larger then the extreme
       then reposition them at the extreme */
    if (firstDim) {
        xStart = xStartK;
        xEnd = xEndK;
        yStart = yStartK;
        yEnd = yEndK;
        zStart = zStartK;
        zEnd = zEndK;
        firstDim = false;
    } else {
        if (xStartK < xStart) xStart = xStartK;
        if (xEndK   > xEnd  ) xEnd   = xEndK;
        if (yStartK < yStart) yStart = yStartK;
        if (yEndK   > yEnd  ) yEnd   = yEndK;
        if (zStartK < zStart) zStart = zStartK;
        if (zEndK   > zEnd  ) zEnd   = zEndK;
    }

    /* Calculate value step size */
    xStep = ((xEnd)-(xStart))/100.0;
    yStep = ((yEnd)-(yStart))/100.0;
    zStep = ((zEnd)-(zStart))/100.0;
    /* Set the range and step of spin boxes */
    ui->doubleSpinBox_xMin->setRange(xStart, xEnd);
    ui->doubleSpinBox_xMin->setSingleStep(xStep);
    ui->doubleSpinBox_xMax->setRange(xStart, xEnd);
    ui->doubleSpinBox_xMax->setSingleStep(xStep);
    ui->doubleSpinBox_yMin->setRange(yStart, yEnd);
    ui->doubleSpinBox_yMin->setSingleStep(yStep);
    ui->doubleSpinBox_yMax->setRange(yStart, yEnd);
    ui->doubleSpinBox_yMax->setSingleStep(yStep);
    ui->doubleSpinBox_zMin->setRange(zStart, zEnd);
    ui->doubleSpinBox_zMin->setSingleStep(zStep);
    ui->doubleSpinBox_zMax->setRange(zStart, zEnd);
    ui->doubleSpinBox_zMax->setSingleStep(zStep);
    /* Update ui */
    if (!(restrictAgentDimension->xminon)) {
        ui->doubleSpinBox_xMin->setValue(xStart);
        ui->horizontalSlider_xMin->setValue(0);
        enableXMin(false);
    } else {
        emit(ui->doubleSpinBox_xMin->setValue(restrictAgentDimension->xmin));
        emit(updateXMin(restrictAgentDimension->xmin));
    }
    if (!(restrictAgentDimension->xmaxon)) {
        ui->doubleSpinBox_xMax->setValue(xEnd);
        ui->horizontalSlider_xMax->setValue(99);
        enableXMax(false);
    } else {
        emit(ui->doubleSpinBox_xMax->setValue(restrictAgentDimension->xmax));
        emit(updateXMax(restrictAgentDimension->xmax));
    }
    if (!(restrictAgentDimension->yminon)) {
        ui->doubleSpinBox_yMin->setValue(yStart);
        ui->horizontalSlider_yMin->setValue(0);
        enableYMin(false);
    } else {
        emit(ui->doubleSpinBox_yMin->setValue(restrictAgentDimension->ymin));
        emit(updateYMin(restrictAgentDimension->ymin));
    }
    if (!(restrictAgentDimension->ymaxon)) {
        ui->doubleSpinBox_yMax->setValue(yEnd);
        ui->horizontalSlider_yMax->setValue(99);
        enableYMax(false);
    } else {
        emit(ui->doubleSpinBox_yMax->setValue(restrictAgentDimension->ymax));
        emit(updateYMax(restrictAgentDimension->ymax));
    }
    if (!(restrictAgentDimension->zminon)) {
        ui->doubleSpinBox_zMin->setValue(zStart);
        ui->horizontalSlider_zMin->setValue(0);
        enableZMin(false);
    } else {
        emit(ui->doubleSpinBox_zMin->setValue(restrictAgentDimension->zmin));
        emit(updateZMin(restrictAgentDimension->zmin));
    }
    if (!(restrictAgentDimension->zmaxon)) {
        ui->doubleSpinBox_zMax->setValue(zEnd);
        ui->horizontalSlider_zMax->setValue(99);
        enableZMax(false);
    } else {
        emit(ui->doubleSpinBox_zMax->setValue(restrictAgentDimension->zmax));
        emit(updateZMax(restrictAgentDimension->zmax));
    }
}

void RestrictAxesDialog::closeEvent(QCloseEvent */*event*/) {
    emit(closed());
}

void RestrictAxesDialog::updateXMin(double d) {
    restrictAgentDimension->xmin = d;
    restrictDimension->xmin = d*(*ratio);
    emit(ui->horizontalSlider_xMin->
            setValue(static_cast<int>((d-xStart)/xStep)));
}

void RestrictAxesDialog::updateXMax(double d) {
    restrictAgentDimension->xmax = d;
    restrictDimension->xmax = d*(*ratio);
    emit(ui->horizontalSlider_xMax->
            setValue(static_cast<int>((d-xStart)/xStep)));
}

void RestrictAxesDialog::updateYMin(double d) {
    restrictAgentDimension->ymin = d;
    restrictDimension->ymin = d*(*ratio);
    emit(ui->horizontalSlider_yMin->
            setValue(static_cast<int>((d-yStart)/yStep)));
}

void RestrictAxesDialog::updateYMax(double d) {
    restrictAgentDimension->ymax = d;
    restrictDimension->ymax = d*(*ratio);
    emit(ui->horizontalSlider_yMax->
            setValue(static_cast<int>((d-yStart)/yStep)));
}

void RestrictAxesDialog::updateZMin(double d) {
    restrictAgentDimension->zmin = d;
    restrictDimension->zmin = d*(*ratio);
    emit(ui->horizontalSlider_zMin->
            setValue(static_cast<int>((d-zStart)/zStep)));
}

void RestrictAxesDialog::updateZMax(double d) {
    restrictAgentDimension->zmax = d;
    restrictDimension->zmax = d*(*ratio);
    emit(ui->horizontalSlider_zMax->
            setValue(static_cast<int>((d-zStart)/zStep)));
}

void RestrictAxesDialog::updateXMinS(int i) {
    if (xminBlock) {
        xminBlock = false;
        return;
    } else {
        xminBlock = true;
    }
    if (i == 0) enableXMin(false);
    else
        enableXMin(true);
    emit(ui->doubleSpinBox_xMin->setValue(i*xStep+xStart));
}

void RestrictAxesDialog::updateXMaxS(int i) {
    if (xmaxBlock) {
        xmaxBlock = false;
        return;
    } else {
        xmaxBlock = true;
    }
    if (i == 99) enableXMax(false);
    else
        enableXMax(true);
    emit(ui->doubleSpinBox_xMax->setValue(i*xStep+xStart));
}

void RestrictAxesDialog::updateYMinS(int i) {
    if (yminBlock) {
        yminBlock = false;
        return;
    } else {
        yminBlock = true;
    }
    if (i == 0) enableYMin(false);
    else
        enableYMin(true);
    emit(ui->doubleSpinBox_yMin->setValue(i*yStep+yStart));
}

void RestrictAxesDialog::updateYMaxS(int i) {
    if (ymaxBlock) {
        ymaxBlock = false;
        return;
    } else {
        ymaxBlock = true;
    }
    if (i == 99) enableYMax(false);
    else
        enableYMax(true);
    emit(ui->doubleSpinBox_yMax->setValue(i*yStep+yStart));
}

void RestrictAxesDialog::updateZMinS(int i) {
    if (zminBlock) {
        zminBlock = false;
        return;
    } else {
        zminBlock = true;
    }
    if (i == 0) enableZMin(false);
    else
        enableZMin(true);
    emit(ui->doubleSpinBox_zMin->setValue(i*zStep+zStart));
}

void RestrictAxesDialog::updateZMaxS(int i) {
    if (zmaxBlock) {
        zmaxBlock = false;
        return;
    } else {
        zmaxBlock = true;
    }
    if (i == 99) enableZMax(false);
    else
        enableZMax(true);
    emit(ui->doubleSpinBox_zMax->setValue(i*zStep+zStart));
}

void RestrictAxesDialog::enableXMin(bool b) {
    restrictDimension->xminon = b;
    restrictAgentDimension->xminon = b;
    ui->label_xmin->setEnabled(b);
    ui->doubleSpinBox_xMin->setEnabled(b);
}

void RestrictAxesDialog::enableXMax(bool b) {
    restrictDimension->xmaxon = b;
    restrictAgentDimension->xmaxon = b;
    ui->label_xmax->setEnabled(b);
    ui->doubleSpinBox_xMax->setEnabled(b);
}

void RestrictAxesDialog::enableYMin(bool b) {
    restrictDimension->yminon = b;
    restrictAgentDimension->yminon = b;
    ui->label_ymin->setEnabled(b);
    ui->doubleSpinBox_yMin->setEnabled(b);
}

void RestrictAxesDialog::enableYMax(bool b) {
    restrictDimension->ymaxon = b;
    restrictAgentDimension->ymaxon = b;
    ui->label_ymax->setEnabled(b);
    ui->doubleSpinBox_yMax->setEnabled(b);
}

void RestrictAxesDialog::enableZMin(bool b) {
    restrictDimension->zminon = b;
    restrictAgentDimension->zminon = b;
    ui->label_zmin->setEnabled(b);
    ui->doubleSpinBox_zMin->setEnabled(b);
}

void RestrictAxesDialog::enableZMax(bool b) {
    restrictDimension->zmaxon = b;
    restrictAgentDimension->zmaxon = b;
    ui->label_zmax->setEnabled(b);
    ui->doubleSpinBox_zMax->setEnabled(b);
}

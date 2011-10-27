#include "restrictaxesdialog.h"
#include "ui_restrictaxesdialog.h"
#include <QDebug>

RestrictAxesDialog::RestrictAxesDialog(Dimension * rd, Dimension * ad, Dimension * rad, double * r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RestrictAxesDialog)
{
    ui->setupUi(this);

    restrictDimension = rd;
    agentDimension = ad;
    restrictAgentDimension = rad;
    ratio = r;

    updatedAgentDimensions();

    connect(ui->doubleSpinBox_xMax, SIGNAL(valueChanged(double)), this, SLOT(updateXMax(double)));
    connect(ui->doubleSpinBox_xMin, SIGNAL(valueChanged(double)), this, SLOT(updateXMin(double)));
    connect(ui->doubleSpinBox_yMax, SIGNAL(valueChanged(double)), this, SLOT(updateYMax(double)));
    connect(ui->doubleSpinBox_yMin, SIGNAL(valueChanged(double)), this, SLOT(updateYMin(double)));
    connect(ui->doubleSpinBox_zMax, SIGNAL(valueChanged(double)), this, SLOT(updateZMax(double)));
    connect(ui->doubleSpinBox_zMin, SIGNAL(valueChanged(double)), this, SLOT(updateZMin(double)));

    connect(ui->horizontalSlider_xMin, SIGNAL(sliderMoved(int)), this, SLOT(updateXMinS(int)));
    connect(ui->horizontalSlider_xMax, SIGNAL(sliderMoved(int)), this, SLOT(updateXMaxS(int)));
    connect(ui->horizontalSlider_yMin, SIGNAL(sliderMoved(int)), this, SLOT(updateYMinS(int)));
    connect(ui->horizontalSlider_yMax, SIGNAL(sliderMoved(int)), this, SLOT(updateYMaxS(int)));
    connect(ui->horizontalSlider_zMin, SIGNAL(sliderMoved(int)), this, SLOT(updateZMinS(int)));
    connect(ui->horizontalSlider_zMax, SIGNAL(sliderMoved(int)), this, SLOT(updateZMaxS(int)));

    connect(this, SIGNAL(accepted()), this, SLOT(close()));
    connect(this, SIGNAL(rejected()), this, SLOT(close()));
}

RestrictAxesDialog::~RestrictAxesDialog()
{
    delete ui;
}

void RestrictAxesDialog::updatedAgentDimensions()
{
    xStart = agentDimension->xmin*1.1;
    xEnd   = agentDimension->xmax*1.1;
    xStep = ((xEnd)-(xStart))/100.0;
    yStart = agentDimension->ymin*1.1;
    yEnd   = agentDimension->ymax*1.1;
    yStep = ((yEnd)-(yStart))/100.0;
    zStart = agentDimension->zmin*1.1;
    zEnd   = agentDimension->zmax*1.1;
    zStep = ((zEnd)-(zStart))/100.0;

    ui->doubleSpinBox_xMin->setRange(xStart,xEnd);
    ui->doubleSpinBox_xMin->setSingleStep(xStep);
    ui->doubleSpinBox_xMax->setRange(xStart,xEnd);
    ui->doubleSpinBox_xMax->setSingleStep(xStep);
    ui->doubleSpinBox_yMin->setRange(yStart,yEnd);
    ui->doubleSpinBox_yMin->setSingleStep(yStep);
    ui->doubleSpinBox_yMax->setRange(yStart,yEnd);
    ui->doubleSpinBox_yMax->setSingleStep(yStep);
    ui->doubleSpinBox_zMin->setRange(zStart,zEnd);
    ui->doubleSpinBox_zMin->setSingleStep(zStep);
    ui->doubleSpinBox_zMax->setRange(zStart,zEnd);
    ui->doubleSpinBox_zMax->setSingleStep(zStep);

    if(!(restrictAgentDimension->xminon)) { ui->horizontalSlider_xMin->setValue(0);   enableXMin(false); }
    else
    {
        emit( ui->doubleSpinBox_xMin->setValue(restrictAgentDimension->xmin) );
        emit( updateXMin(restrictAgentDimension->xmin) );
    }
    if(!(restrictAgentDimension->xmaxon)) { ui->horizontalSlider_xMax->setValue(99);  enableXMax(false); }
    else
    {
        emit( ui->doubleSpinBox_xMax->setValue(restrictAgentDimension->xmax) );
        emit( updateXMax(restrictAgentDimension->xmax) );
    }
    if(!(restrictAgentDimension->yminon)) { ui->horizontalSlider_yMin->setValue(0);   enableYMin(false); }
    else
    {
        emit( ui->doubleSpinBox_yMin->setValue(restrictAgentDimension->ymin) );
        emit( updateYMin(restrictAgentDimension->ymin) );
    }
    if(!(restrictAgentDimension->ymaxon)) { ui->horizontalSlider_yMax->setValue(99);  enableYMax(false); }
    else
    {
        emit( ui->doubleSpinBox_yMax->setValue(restrictAgentDimension->ymax) );
        emit( updateYMax(restrictAgentDimension->ymax) );
    }
    if(!(restrictAgentDimension->zminon)) { ui->horizontalSlider_zMin->setValue(0);   enableZMin(false); }
    else
    {
        emit( ui->doubleSpinBox_zMin->setValue(restrictAgentDimension->zmin) );
        emit( updateZMin(restrictAgentDimension->zmin) );
    }
    if(!(restrictAgentDimension->zmaxon)) { ui->horizontalSlider_zMax->setValue(99);  enableZMax(false); }
    else
    {
        emit( ui->doubleSpinBox_zMax->setValue(restrictAgentDimension->zmax) );
        emit( updateZMax(restrictAgentDimension->zmax) );
    }
}

void RestrictAxesDialog::closeEvent(QCloseEvent */*event*/)
{
    emit( closed() );
}

void RestrictAxesDialog::updateXMin(double d)
{
    restrictAgentDimension->xmin = d;
    restrictDimension->xmin = d*(*ratio);
    emit( ui->horizontalSlider_xMin->setValue((int)((d-xStart)/xStep)) );
}
void RestrictAxesDialog::updateXMax(double d)
{
    restrictAgentDimension->xmax = d;
    restrictDimension->xmax = d*(*ratio);
    emit( ui->horizontalSlider_xMax->setValue((int)((d-xStart)/xStep)) );
}
void RestrictAxesDialog::updateYMin(double d)
{
    restrictAgentDimension->ymin = d;
    restrictDimension->ymin = d*(*ratio);
    emit( ui->horizontalSlider_yMin->setValue((int)((d-yStart)/yStep)) );
}
void RestrictAxesDialog::updateYMax(double d)
{
    restrictAgentDimension->ymax = d;
    restrictDimension->ymax = d*(*ratio);
    emit( ui->horizontalSlider_yMax->setValue((int)((d-yStart)/yStep)) );
}
void RestrictAxesDialog::updateZMin(double d)
{
    restrictAgentDimension->zmin = d;
    restrictDimension->zmin = d*(*ratio);
    emit( ui->horizontalSlider_zMin->setValue((int)((d-zStart)/zStep)) );
}
void RestrictAxesDialog::updateZMax(double d)
{
    restrictAgentDimension->zmax = d;
    restrictDimension->zmax = d*(*ratio);
    emit( ui->horizontalSlider_zMax->setValue((int)((d-zStart)/zStep)) );
}

void RestrictAxesDialog::updateXMinS(int i)
{
    if(i==0) enableXMin(false); else enableXMin(true);
    emit( ui->doubleSpinBox_xMin->setValue(i*xStep+xStart) );
}
void RestrictAxesDialog::updateXMaxS(int i)
{
    if(i==99) enableXMax(false); else enableXMax(true);
    emit( ui->doubleSpinBox_xMax->setValue(i*xStep+xStart) );
}
void RestrictAxesDialog::updateYMinS(int i)
{
    if(i==0) enableYMin(false); else enableYMin(true);
    emit( ui->doubleSpinBox_yMin->setValue(i*yStep+yStart) );
}
void RestrictAxesDialog::updateYMaxS(int i)
{
    if(i==99) enableYMax(false); else enableYMax(true);
    emit( ui->doubleSpinBox_yMax->setValue(i*yStep+yStart) );
}
void RestrictAxesDialog::updateZMinS(int i)
{
    if(i==0) enableZMin(false); else enableZMin(true);
    emit( ui->doubleSpinBox_zMin->setValue(i*zStep+zStart) );
}
void RestrictAxesDialog::updateZMaxS(int i)
{
    if(i==99) enableZMax(false); else enableZMax(true);
    emit( ui->doubleSpinBox_zMax->setValue(i*zStep+zStart) );
}

void RestrictAxesDialog::enableXMin(bool b)
{
    restrictDimension->xminon = b;
    restrictAgentDimension->xminon = b;
    ui->label_xmin->setEnabled(b);
    ui->doubleSpinBox_xMin->setEnabled(b);
}
void RestrictAxesDialog::enableXMax(bool b)
{
    restrictDimension->xmaxon = b;
    restrictAgentDimension->xmaxon = b;
    ui->label_xmax->setEnabled(b);
    ui->doubleSpinBox_xMax->setEnabled(b);
}
void RestrictAxesDialog::enableYMin(bool b)
{
    restrictDimension->yminon = b;
    restrictAgentDimension->yminon = b;
    ui->label_ymin->setEnabled(b);
    ui->doubleSpinBox_yMin->setEnabled(b);
}
void RestrictAxesDialog::enableYMax(bool b)
{
    restrictDimension->ymaxon = b;
    restrictAgentDimension->ymaxon = b;
    ui->label_ymax->setEnabled(b);
    ui->doubleSpinBox_yMax->setEnabled(b);
}
void RestrictAxesDialog::enableZMin(bool b)
{
    restrictDimension->zminon = b;
    restrictAgentDimension->zminon = b;
    ui->label_zmin->setEnabled(b);
    ui->doubleSpinBox_zMin->setEnabled(b);
}
void RestrictAxesDialog::enableZMax(bool b)
{
    restrictDimension->zmaxon = b;
    restrictAgentDimension->zmaxon = b;
    ui->label_zmax->setEnabled(b);
    ui->doubleSpinBox_zMax->setEnabled(b);
}

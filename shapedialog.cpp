#include <QtGui>
#include <QHBoxLayout>
#include "shapedialog.h"

ShapeDialog::ShapeDialog(VisualSettingsModel *vsm, QWidget *parent)
    : QDialog(parent)
{
    vsmodel = vsm;

    shapeComboBox = new QComboBox();
    shapeComboBox->addItems(shape.Shapes());
    dimensionSpinBox = new QDoubleSpinBox();
    dimensionSpinBox->setMaximum(9999.99);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);

    connect(this, SIGNAL(setShapeComboBox(int)), shapeComboBox, SLOT(setCurrentIndex(int)));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(shapeComboBox);
    mainLayout->addWidget(dimensionSpinBox);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle("Shape Editor");
}

void ShapeDialog::setShape(Shape s)
{
    shape = s;

    int index = shape.Shapes().indexOf(shape.getShape());
    if(index == -1) index = 0;
    emit ( setShapeComboBox(index) );

    dimensionSpinBox->setValue(shape.getDimension());
}

Shape ShapeDialog::getShape()
{
    shape.setShape(shapeComboBox->currentText());
    shape.setDimension(dimensionSpinBox->value());
    return shape;
}

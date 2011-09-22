#ifndef SHAPEDIALOG_H
#define SHAPEDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include "visualsettingsmodel.h"
#include "shape.h"

class QDialogButtonBox;

class ShapeDialog : public QDialog
{
    Q_OBJECT

public:
    ShapeDialog(VisualSettingsModel * vsm, QWidget *parent = 0);
    void setShape(Shape s);
    Shape getShape();

signals:
    void setShapeComboBox(int i);
    void setDimensionSpinBox(double d);

private:
    VisualSettingsModel * vsmodel;
    QComboBox * shapeComboBox;
    QDoubleSpinBox * dimensionSpinBox;
    QDialogButtonBox *buttonBox;
    Shape shape;
};

#endif // SHAPEDIALOG_H

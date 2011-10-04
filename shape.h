#ifndef SHAPE_H
#define SHAPE_H

#include <QMetaType>
#include <QPainter>

class Shape
{
public:
    enum EditMode { Editable, ReadOnly };

    Shape();
    void setShape(QString s) { shape = s; }
    QString getShape() { return shape; }
    QStringList Shapes() { return shapes; }
    double getDimension() { return dimension; }
    void setDimension(double d) { dimension = d; }
    double getDimensionY() { return dimensionY; }
    void setDimensionY(double d) { dimensionY = d; }
    double getDimensionZ() { return dimensionZ; }
    void setDimensionZ(double d) { dimensionZ = d; }
    QString getDimensionVariable() { return dimensionVariable; }
    void setDimensionVariable(QString d) { dimensionVariable = d; }
    QString getDimensionVariableY() { return dimensionVariableY; }
    void setDimensionVariableY(QString d) { dimensionVariableY = d; }
    QString getDimensionVariableZ() { return dimensionVariableZ; }
    void setDimensionVariableZ(QString d) { dimensionVariableZ = d; }
    void setUseVariable(bool b) { useVariable = b; }
    bool getUseVariable() { return useVariable; }
    void setUseVariableY(bool b) { useVariableY = b; }
    bool getUseVariableY() { return useVariableY; }
    void setUseVariableZ(bool b) { useVariableZ = b; }
    bool getUseVariableZ() { return useVariableZ; }

    void paint(QPainter *painter, const QRect &rect,
                const QPalette &palette, EditMode mode) const;

private:
    QString shape;
    double dimension;
    double dimensionY;
    double dimensionZ;
    QString dimensionVariable;
    QString dimensionVariableY;
    QString dimensionVariableZ;
    QStringList shapes;
    bool useVariable;
    bool useVariableY;
    bool useVariableZ;
};

Q_DECLARE_METATYPE(Shape)

#endif // SHAPE_H

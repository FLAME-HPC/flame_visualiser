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
    QString getDimensionVariable() { return dimensionVariable; }
    void setDimensionVariable(QString d) { dimensionVariable = d; }
    void setUseValue(bool b) { useValue = b; }
    bool getUseValue() { return useValue; }
    void setUseVariable(bool b) { useVariable = b; }
    bool getUseVariable() { return useVariable; }

    void paint(QPainter *painter, const QRect &rect,
                const QPalette &palette, EditMode mode) const;

private:
    QString shape;
    double dimension;
    QString dimensionVariable;
    QStringList shapes;
    bool useValue;
    bool useVariable;
};

Q_DECLARE_METATYPE(Shape)

#endif // SHAPE_H

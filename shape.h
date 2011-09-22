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

    void paint(QPainter *painter, const QRect &rect,
                const QPalette &palette, EditMode mode) const;

private:
    QString shape;
    double dimension;
    QStringList shapes;
};

Q_DECLARE_METATYPE(Shape)

#endif // SHAPE_H

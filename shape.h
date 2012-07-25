/*!
 * \file shape.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for shape
 */
#ifndef SHAPE_H_
#define SHAPE_H_

#include <QMetaType>
#include <QPainter>

class Shape {
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
    void setFromCentreX(bool b) { fromCentreX = b; }
    bool getFromCentreX() { return fromCentreX; }
    void setFromCentreY(bool b) { fromCentreY = b; }
    bool getFromCentreY() { return fromCentreY; }
    void setFromCentreZ(bool b) { fromCentreZ = b; }
    bool getFromCentreZ() { return fromCentreZ; }

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
    bool fromCentreX;
    bool fromCentreY;
    bool fromCentreZ;
};

Q_DECLARE_METATYPE(Shape)

#endif  // SHAPE_H_

#ifndef SHAPEDIALOG_H
#define SHAPEDIALOG_H

#include <QDialog>
#include "visualsettingsmodel.h"
#include "shape.h"
#include "agenttype.h"
#include "ui_shapedialog.h"

class ShapeDialog : public QDialog, public Ui::ShapeDialog
{
    Q_OBJECT

public:
    ShapeDialog(QList<AgentType> * ats = 0, VisualSettingsModel * vsm = 0, int r = 0, QWidget *parent = 0);
    void setShape(Shape s);
    Shape getShape();

private slots:
    void shapeUpdated(QString);

private:
    void enableYZ(bool e);
    VisualSettingsModel * vsmodel;
    Shape shape;
    QList<AgentType> * agentTypes;
    int row;
};

#endif // SHAPEDIALOG_H

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QMainWindow>
#include <QGLWidget>
#include <QTime>
#include "agent.h"
#include "visualsettingsmodel.h"

class QTimer;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
    void update_agents(QList<Agent> * a);
    void set_rules(VisualSettingsModel * m);
    void set_ratio(double * r) { ratio = r; }
    void reset_camera();
    void window_closed();
    QString getName() { return name; }
    void setName(QString n) { name = n; }

signals:
    void increase_iteration();
    void decrease_iteration();
    void visual_window_closed();

protected:
    void paintEvent(QPaintEvent *event);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void timeOut();
    void closeEvent(QCloseEvent *event);

private:
    QString name;
    QList<Agent> * agents;
    bool block;
    float xrotate;
    float yrotate;
    float zmove;
    int x_last_position;
    int y_last_position;
    int drawpoints;
    int iteration;
    bool light;
    double alphaA;
    bool spinup, spindown, spinleft, spinright;
    bool itforward, itback;
    QTimer *timer;
    QTime time;
    VisualSettingsModel * model;
    double *ratio;
};

#endif // GLWIDGET_H

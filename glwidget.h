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
    void reset_camera();
    QString getName() { return name; }
    void setName(QString n) { name = n; }
    void setIteration(int * i) { iteration = i; }
    void setConfigPath(QString * cp) { configpath = cp; }
    void setTimeDisplayed(bool * b) { displayTime = b; }
    void setTimeString(QString * t) { timeString = t; }

public slots:
    void animate();
    void iterationLoaded();
    void nextIteration();
    void takeSnapshot();
    void takeAnimation(bool);
    void updateImagesLocation(QString);
    void stopAnimation();

signals:
    void increase_iteration();
    void decrease_iteration();
    void visual_window_closed();
    void imageStatus(QString);

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
    void processSelection(int mx, int my);
    void drawAgents(GLenum mode);
    void drawCube(float sizeX, float sizeY, float sizeZ);
    QString name;
    QList<Agent> * agents;
    bool block;
    float xrotate;
    float yrotate;
    float xmove;
    float ymove;
    float zmove;
    int x_last_position;
    int y_last_position;
    int drawpoints;
    int * iteration;
    bool light;
    double alphaA;
    bool spinup, spindown, spinleft, spinright;
    bool itforward, itback;
    QTimer *timer;
    QTime time;
    VisualSettingsModel * model;
    bool animation;
    bool locked;
    bool animationImages;
    bool imageLock;
    QString imagesLocation;
    QString * configpath;
    bool pickOn;
    float window_ratio;
    bool * displayTime;
    QString * timeString;
    float zNear; ///< The near clipping plane
    bool clippingOn;
    int windowWidth, windowHeight;
    QHash<int, Agent*> nameAgents;
    Agent nameAgent; ///< A copy of the picked agent
    bool drawNameAgent;
    bool moveOn;
};

#endif // GLWIDGET_H

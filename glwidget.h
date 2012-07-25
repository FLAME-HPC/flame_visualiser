/*!
 * \file glwidget.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for OpenGL widget
 */
#ifndef GLWIDGET_H_
#define GLWIDGET_H_

#include <QMainWindow>
#include <QGLWidget>
#include <QTime>
#include "./agent.h"
#include "./visualsettingsmodel.h"
#include "./dimension.h"
#include "./timescale.h"
#if QT_VERSION >= 0x040800  // If Qt version is 4.8 or higher
    #ifdef Q_WS_MAC  // If Mac
        #include <OpenGL/glu.h>
    #else
        #include <GL/glu.h>
    #endif
#endif

class QTimer;

class GLWidget : public QGLWidget {
    Q_OBJECT

  public:
    GLWidget(float * xr, float * yr, float * xm, float * ym, float * zm,
            Dimension * rd, float * oz, QWidget *parent = 0);
    ~GLWidget();
    void update_agents(QList<Agent*> * a);
    void set_rules(VisualSettingsModel * m);
    void reset_camera();
    QString getName() { return name; }
    void setName(QString n) { name = n; }
    void setIteration(int * i) { iteration = i; }
    void setConfigPath(QString * cp) { configpath = cp; }
    void setTimeScale(TimeScale * ts) { timeScale = ts; }
    void setTimeString(QString * t) { timeString = t; }
    void setDimension(int d);

  public slots:
    void iterationLoaded();
    void nextIteration();
    void takeSnapshot();
    void takeAnimation(bool);
    void updateImagesLocation(QString);
    void stopAnimation();
    void startAnimation();
    void restrictAxes(bool);
    void updateDelayTime(int);

  signals:
    void increase_iteration();
    void decrease_iteration();
    void visual_window_closed();
    void imageStatus(QString);
    void signal_stopAnimation();
    void signal_startAnimation();

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

  private slots:
    void unlockDelayTime();

  private:
    void processSelection(int mx, int my);
    void drawAgents(GLenum mode);
    void drawCube(float sizeX, float sizeY, float sizeZ);
    void drawSphere(double size);
    QString name;
    QList<Agent*> * agents;
    bool block;
    float * xrotate;
    float * yrotate;
    float * xmove;
    float * ymove;
    float * zmove;
    float * orthoZoom;
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
    QTimer *delayTimer;
    VisualSettingsModel * model;
    bool animation;
    bool locked;
    bool animationImages;
    bool imageLock;
    QString imagesLocation;
    QString * configpath;
    bool pickOn;
    float window_ratio;
    TimeScale * timeScale;
    QString * timeString;
    float zNear;  /*!< \brief The near clipping plane */
    bool clippingOn;
    int windowWidth, windowHeight;
    QHash<int, RuleAgent*> nameAgents;
    Agent nameAgent;  /*!< \brief A copy of the picked agent */
    bool drawNameAgent;
    bool moveOn;
    Dimension * restrictDimension;
    bool restrictAxesOn;
    bool delayLock;
    int delayTime;
    int dimension;
    GLuint nPartsList; /*!< \brief Use with display lists */
    GLuint SPHERE_4;  /*!< \brief Sphere with grade 4 */
    GLuint SPHERE_8;  /*!< \brief Sphere with grade 8 */
    GLuint SPHERE_16;  /*!< \brief Sphere with grade 16 */
    GLuint SPHERE_32;  /*!< \brief Sphere with grade 32 */
    GLuint SPHERE_64;  /*!< \brief Sphere with grade 64 */
    GLUquadricObj * dl_qobj;
};

#endif  // GLWIDGET_H_

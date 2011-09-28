#include <QtGui>
#include <QKeyEvent>
#include <QTimer>
#include "GLWidget.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include "visualsettingsmodel.h"
#include "visualsettingsitem.h"
#include "condition.h"

//Q_IMPORT_PLUGIN(qjpeg)

#define ZMOVE -3.0

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    agents = 0;
    setMouseTracking(true);
    xrotate = 0.0;
    yrotate = 0.0;
    zmove = ZMOVE;
    x_last_position = 0;
    y_last_position = 0;
    drawpoints = 1;
    light = true;
    alphaA = 1.0;
    spinup = false;
    spindown = false;
    spinleft = false;
    spinright = false;
    itforward = false;
    itback = false;
    block = false;
    animation = false;
    locked = false;
    animationImages = false;
    imageLock = false;

    time = QTime::currentTime();
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
}

GLWidget::~GLWidget()
{
    emit( visual_window_closed() );
}

void GLWidget::closeEvent(QCloseEvent *event)
 {
     emit( visual_window_closed() );
     event->accept();
 }

void GLWidget::updateImagesLocation(QString s)
{
    //qDebug() << s;
    imagesLocation = s;
}

void GLWidget::takeSnapshot()
{
    imageLock = true;

    QString filepath;
    filepath.append(*configpath);
    filepath.append("/");
    filepath.append(imagesLocation);

    QDir filedir(filepath);

    QString filename = filedir.canonicalPath();
    filename.append("/");
    filename.append(QString("%1").arg(*iteration));
    filename.append(".jpg");

    qDebug() << filename;

    QImage i = this->grabFrameBuffer();
    QImageWriter writer(filename, "JPG");
    writer.setQuality(90);
    if( !writer.write(i) )
    {
        qDebug() << writer.errorString() << filename;
        emit( imageStatus(QString("Error: %1 %2").arg(writer.errorString(),filename)) );
    }
    else
    {
        emit( imageStatus(QString("Saved: %1").arg(filename)) );
    }
    imageLock = false;
}

void GLWidget::stopAnimation()
{
    animation = false;
}

void GLWidget::animate()
{
    animation = !animation;
    //qDebug() << "animation" << animation;
    //if(animation) emit( increase_iteration() );
}

void GLWidget::takeAnimation(bool b)
{
    animationImages = b;
    //qDebug() << "animationImages" << b;
}

void GLWidget::iterationLoaded()
{
    locked = false;
    //emit( updateGL() );
    //if(animation) QTimer::singleShot(2000, this, SLOT(nextIteration()));
    //if(animation) emit( increase_iteration() );
}

void GLWidget::nextIteration()
{
    emit( increase_iteration() );
}

void GLWidget::update_agents(QList<Agent> * a)
{
    agents = a;
}

void GLWidget::set_rules(VisualSettingsModel * m)
{
    model = m;
    //qDebug() << "set_rules" << m;
}

void GLWidget::reset_camera()
{
    xrotate = 0.0;
    yrotate = 0.0;
    zmove = ZMOVE;
}

void GLWidget::initializeGL()
{
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 }; /* Reflectivness properties of material */
    GLfloat mat_shininess[] = { 50.0 }; /* Size & Brightness of highlight, 0-128 */
    GLfloat light_ambient[] =  { 0.0, 0.0, 0.0, 1.0 }; /* Ambient light from the light source */
    GLfloat light_diffuse[] =  { 1.0, 1.0, 1.0, 1.0 }; /* Colour of the light from light source */
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; /* Colour of highlight on object, should use same as diffuse */
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; /* Light source position */
    GLfloat ambient_light[] =  { 0.6, 0.6, 0.6, 1.0 }; /* Global ambient light, use this to lighten objects */
    //GLfloat ambient_light[] =  { 1.0, 1.0, 1.0, 1.0 }; /* Global ambient light, use this to lighten objects */

    //glDisable(GL_TEXTURE_2D);
    //glDisable(GL_DEPTH_TEST);
    //glDisable(GL_COLOR_MATERIAL);
    //glEnable(GL_BLEND);
    //glEnable(GL_POLYGON_SMOOTH);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(1, 1, 1, 0);
    glShadeModel( GL_SMOOTH );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    /* Lighting */
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void GLWidget::resizeGL(int w, int h)
{
    float window_ratio = (GLfloat)((float)w/(float)h);

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(0, 0, w, h); // set origin to centre
    gluPerspective( 45.0f, window_ratio, 0.1f, 20.0f );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL()
{
    bool drawAgent;
    //double ratio = 0.008;
    double size = 0.03;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //glColor3f(1,0,0);
    /*glBegin(GL_POLYGON);
    glVertex2f(0,0);
    glVertex2f(100,500);
    glVertex2f(500,100);
    glEnd();*/

    //qDebug("paintGL");



    glPushMatrix();

    glTranslatef(0.0, 0.0, zmove);
    glRotatef(yrotate, 1.0f, 0.0f, 0.0f);
    glRotatef(xrotate, 0.0f, 0.0f, 1.0f);

    int grade = 16;//16;
    int style = 0;
    GLUquadricObj * qobj = gluNewQuadric();
    if(style==0)
    {
        gluQuadricDrawStyle(qobj, GLU_FILL); /* smooth shaded */
        gluQuadricNormals(qobj, GLU_SMOOTH);
    }
    else if(style==1)
    {
        gluQuadricDrawStyle(qobj, GLU_FILL); /* flat shaded */
        gluQuadricNormals(qobj, GLU_FLAT);
    }
    else if(style==2)
    {
        gluQuadricDrawStyle(qobj, GLU_LINE); /* all polygons wireframe */
        gluQuadricNormals(qobj, GLU_NONE);
    }

    //qDebug() << "agents count: " << agents->count();

    if(agents != 0)
    {
        //qDebug("agents visual = %d", agents->size());

        for(int i = 0; i < agents->size(); i++)
        {
            if(model == NULL) qDebug() << "WARNING: model is NULL";
            else
            {
                //qDebug() << model->getRules().count();
                for(int j = 0; j < model->getRules().count(); j++)
                {
                    //qDebug() << j << model->getRules().at(j)->agentType();
                    VisualSettingsItem * rule = model->getRules()[j];
                    if(QString::compare(agents->at(i).agentType, rule->agentType()) == 0)
                    {
                        drawAgent = true;
                        if(rule->condition().enable)
                        {
                            drawAgent = false;

                            for(int k = 0; k < agents->at(i).tags.count(); k++)
                            {
                                if(QString::compare(rule->condition().variable,agents->at(i).tags.at(k)) == 0)
                                {
                                    if(rule->condition().op == "==" && agents->at(i).values.at(k).toDouble() == rule->condition().value) drawAgent = true;
                                    else if(rule->condition().op == "!=" && agents->at(i).values.at(k).toDouble() != rule->condition().value) drawAgent = true;
                                    else if(rule->condition().op == ">" && agents->at(i).values.at(k).toDouble() > rule->condition().value) drawAgent = true;
                                    else if(rule->condition().op == "<" && agents->at(i).values.at(k).toDouble() < rule->condition().value) drawAgent = true;
                                    else if(rule->condition().op == ">=" && agents->at(i).values.at(k).toDouble() >= rule->condition().value) drawAgent = true;
                                    else if(rule->condition().op == "<=" && agents->at(i).values.at(k).toDouble() <= rule->condition().value) drawAgent = true;
                                }
                            }
                        }
                        if(drawAgent)
                        {
                            size = rule->shape().getDimension();
                            for(int k = 0; k < agents->at(i).tags.count(); k++)
                            {
                                if(QString::compare(rule->x().positionVariable,agents->at(i).tags.at(k)) == 0) x = agents->at(i).values.at(k).toDouble() + rule->x().opValue;
                                if(QString::compare(rule->y().positionVariable,agents->at(i).tags.at(k)) == 0) y = agents->at(i).values.at(k).toDouble() + rule->y().opValue;
                                if(QString::compare(rule->z().positionVariable,agents->at(i).tags.at(k)) == 0) z = agents->at(i).values.at(k).toDouble() + rule->z().opValue;
                            }

                            glPushMatrix();

                            glTranslatef(x*(*ratio), y*(*ratio), z*(*ratio));

                            GLfloat mat_ambientA[] = { rule->colour().red()/255.0, rule->colour().green()/255.0, rule->colour().blue()/255.0, alphaA };

                            if(light) glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambientA );
                            else glColor4fv(mat_ambientA);

                            if(QString::compare("sphere",rule->shape().getShape()) == 0) gluSphere(qobj, size*(*ratio), grade, grade);
                            else if(QString::compare("point",rule->shape().getShape()) == 0)
                            {
                                glPointSize((int)size);
                                glBegin(GL_POINTS);
                                glVertex3f(0.0, 0.0, 0.0);
                                glEnd();
                            }
                            else if(QString::compare("cube",rule->shape().getShape()) == 0)
                            {
                                  glBegin(GL_QUADS);                 // Draw using quads
                                  glNormal3f( 0.0f, 0.5f, 0.0f);
                                  glVertex3f( size, size, -size); // Top-right of the quad (Top)
                                  glVertex3f(-size, size, -size); // Top-left of the quad (Top)
                                  glVertex3f(-size, size,  size); // Bottom-left of the quad (Top)
                                  glVertex3f( size, size,  size); // Bottom-right of the quad (Top)

                                  glNormal3f( 0.0f,-size, 0.0f);
                                  glVertex3f( size, -size,  size); // Top-right of the quad (Bottom)
                                  glVertex3f(-size, -size,  size); // Top-left of the quad (Bottom)
                                  glVertex3f(-size, -size, -size); // Bottom-left of the quad (Bottom)
                                  glVertex3f( size, -size, -size); // Bottom-right of the quad (Bottom)

                                  glNormal3f( 0.0f, 0.0f, size);
                                  glVertex3f( size,  size, size);  // Top-right of the quad (Front)
                                  glVertex3f(-size,  size, size);  // Top-left of the quad (Front)
                                  glVertex3f(-size, -size, size);  // Bottom-left of the quad (Front)
                                  glVertex3f( size, -size, size);  // Bottom-right of the quad (Front)

                                  glNormal3f( 0.0f, 0.0f,-size);
                                  glVertex3f( size, -size, -size); // Bottom-left of the quad (Back)
                                  glVertex3f(-size, -size, -size); // Bottom-right of the quad (Back)
                                  glVertex3f(-size,  size, -size); // Top-right of the quad (Back)
                                  glVertex3f( size,  size, -size); // Top-left of the quad (Back)

                                  glNormal3f(-size, 0.0f, 0.0f);
                                  glVertex3f(-size,  size,  size); // Top-right of the quad (Left)
                                  glVertex3f(-size,  size, -size); // Top-left of the quad (Left)
                                  glVertex3f(-size, -size, -size); // Bottom-left of the quad (Left)
                                  glVertex3f(-size, -size,  size); // Bottom-right of the quad (Left)

                                  glNormal3f( size, 0.0f, 0.0f);
                                  glVertex3f( size,  size, -size); // Top-right of the quad (Right)
                                  glVertex3f( size,  size,  size); // Top-left of the quad (Right)
                                  glVertex3f( size, -size,  size); // Bottom-left of the quad (Right)
                                  glVertex3f( size, -size, -size); // Bottom-right of the quad (Right)
                                  glEnd();   // Done drawing the color cube
                            }
                            glPopMatrix();
                        }
                    }
                }
            }
        }
    }

    glPopMatrix();

    if(spinup)    yrotate -= 1.0;
    if(spindown)  yrotate += 1.0;
    if(spinleft)  xrotate -= 1.0;
    if(spinright) xrotate += 1.0;

    // Framerate control
    int delay = time.msecsTo(QTime::currentTime());
    if (delay == 0)
        delay = 1;
    time = QTime::currentTime();
    timer->start(qMax(0, 20 - delay));

    if(animation && !locked && !imageLock)
    {
        if(animationImages) takeSnapshot();
        emit( increase_iteration() );
    }
}

void GLWidget::paintEvent(QPaintEvent * /*event*/)
{
    /*QPainter painter(this);
    drawBackground(&painter);
    paintGL();
    drawLegend(&painter);*/
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    x_last_position = event->x();
    y_last_position = event->y();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    // if left button
    if (event->buttons() & Qt::LeftButton)
    {
        xrotate -= (float)((x_last_position - event->x())/2.0);
        yrotate -= (float)((y_last_position - event->y())/2.0);
    }
    else if (event->buttons() & Qt::RightButton)
    {
        zmove += (float)((y_last_position - event->y())/100.0);
    }
    x_last_position = event->x();
    y_last_position = event->y();
    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent * event)
{
    zmove += (float)(event->delta()/500.0);
    updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_Left:
            spinleft = true;
            break;
        case Qt::Key_Right:
            spinright = true;
            break;
        case Qt::Key_Up:
            spinup = true;
            break;
        case Qt::Key_Down:
            spindown = true;
            break;
        case Qt::Key_Z:
            emit( decrease_iteration() );
            break;
        case Qt::Key_X:
            emit( increase_iteration() );
            break;
        case Qt::Key_A:
            animate();
            break;
        default:
            //if(animation) emit( increase_iteration() );
            event->ignore();
            break;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent* event)
{
    switch(event->key())
    {
        case Qt::Key_Left:
            spinleft = false;
            break;
        case Qt::Key_Right:
            spinright = false;
            break;
        case Qt::Key_Up:
            spinup = false;
            break;
        case Qt::Key_Down:
            spindown = false;
            break;
        default:
            event->ignore();
            break;
    }
}

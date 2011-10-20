#include <QtGui>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include "glwidget.h"
#include "visualsettingsmodel.h"
#include "visualsettingsitem.h"
#include "condition.h"
#include <QtPlugin>
#include "agentdialog.h"

/* If Windows then include importing plugins for static compilation */
#ifdef Q_WS_WIN
    Q_IMPORT_PLUGIN(qjpeg)
#endif

/* The default camera zoom */
#define ZMOVE -3.0

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    agents = 0;
    setMouseTracking(true);
    xrotate = 0.0;
    yrotate = 0.0;
    xmove = 0.0;
    ymove = 0.0;
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
    pickOn = false;
    zNear = 0.1f;
    clippingOn = false;
    drawNameAgent = false;
    moveOn = false;

    time = QTime::currentTime();
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    setAutoFillBackground(false); // To instruct OpenGL not to paint a background for the widget when QPainter::begin() is called.
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
}

void GLWidget::takeAnimation(bool b)
{
    animationImages = b;
}

void GLWidget::iterationLoaded()
{
    locked = false;
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
    //glEnable( GL_DEPTH_TEST );
    //glDepthFunc( GL_LEQUAL );
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
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLWidget::resizeGL(int w, int h)
{
    window_ratio = (GLfloat)((float)w/(float)h);
    windowWidth = w;
    windowHeight = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(0, 0, w, h); // set origin to centre
    gluPerspective( 45.0f, window_ratio, zNear, 20.0f );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();

    glTranslatef(xmove, ymove, zmove);
    glRotatef(yrotate, 1.0f, 0.0f, 0.0f);
    glRotatef(xrotate, 0.0f, 0.0f, 1.0f);

    drawAgents(GL_RENDER);

    glPopMatrix();

    //glFlush();

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

void GLWidget::drawAgents(GLenum mode)
{
    double size = 0.0;
    double sizeY = 0.0;
    double sizeZ = 0.0;
    //int grade;
    int name = 0;
    nameAgents.clear();

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

    for(int pass = 1; pass < 4; pass++)
    {
        if(pass == 1)
        {
            // if first pass
            glEnable(GL_DEPTH_TEST); glDepthFunc( GL_LEQUAL );
            glDisable(GL_BLEND);
        }
        else if(pass == 2)
        {
            // if second pass
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        /* For every rule */
        for(int j = 0; j < model->getRules().count(); j++)
        {
            VisualSettingsItem * rule = model->getRules()[j];

            /* For every agent associated with the current rule */
            for(int i = 0; i < rule->agents.size(); i++)
            {
                /* If the object is opaque and first pass, or
                   if the object is transparent and is the second or third pass */
                if(((rule->colour().alphaF() >= 0.95 && pass == 1)||(rule->colour().alphaF() < 0.95 && pass >= 2)) )
                {
                    glPushMatrix();

                    glTranslatef(rule->agents.at(i).x, rule->agents.at(i).y, rule->agents.at(i).z);

                    GLfloat mat_ambientA[] = { rule->colour().redF(), rule->colour().greenF(), rule->colour().blueF(), rule->colour().alphaF() };//alphaA };

                    if(light) glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambientA );
                    else glColor4fv(mat_ambientA);

                    size = rule->agents.at(i).shapeDimension;
                    sizeY = rule->agents.at(i).shapeDimensionY;
                    sizeZ = rule->agents.at(i).shapeDimensionZ;

                    if(QString::compare("sphere",rule->shape().getShape()) == 0)
                    {
                        // if the sphere is large then draw with a higher grade
                        /*grade = 16;
                        if(size > 0.1) grade = 32;*/
                        int grade = rule->shape().getQuality();

                        glEnable(GL_CULL_FACE);
                        if(pass==2)
                        {
                            if(mode == GL_SELECT) { glLoadName(0); }
                            // Draw back face
                            glCullFace(GL_FRONT);
                            gluSphere(qobj, size, grade, grade);
                        }
                        else
                        {
                            if(mode == GL_SELECT) { glLoadName(++name); nameAgents.insert(name, &rule->agents[i]); }
                            // Draw front face
                            glCullFace(GL_BACK);
                            gluSphere(qobj, size, grade, grade);
                        }
                        glDisable(GL_CULL_FACE);
                    }
                    else if(QString::compare("point",rule->shape().getShape()) == 0)
                    {
                        if(pass != 2)
                        {
                            if(mode == GL_SELECT) { glLoadName(++name); nameAgents.insert(name, &rule->agents[i]); }

                            glDisable(GL_LIGHTING);
                            glColor4fv(mat_ambientA);
                            glPointSize((int)rule->shape().getDimension());
                            glBegin(GL_POINTS);
                            glVertex3f(0.0, 0.0, 0.0);
                            glEnd();
                            glEnable(GL_LIGHTING);
                        }
                    }
                    else if(QString::compare("cube",rule->shape().getShape()) == 0)
                    {
                        glEnable(GL_CULL_FACE);
                        if(pass==2)
                        {
                            if(mode == GL_SELECT) { glLoadName(0); }
                            // Draw back face
                            glCullFace(GL_FRONT);
                            drawCube(size, sizeY, sizeZ);
                        }
                        else
                        {
                            if(mode == GL_SELECT) { glLoadName(++name); nameAgents.insert(name, &rule->agents[i]); }
                            // Draw front face
                            glCullFace(GL_BACK);
                            drawCube(size, sizeY, sizeZ);
                        }
                        glDisable(GL_CULL_FACE);
                    }

                    //if(mode == GL_SELECT) glPopName();
                    glPopMatrix();
                }
            }
        }
    }
}

void GLWidget::drawCube(float size, float sizeY, float sizeZ)
{
    glBegin(GL_QUADS);                 // Draw using quads
    glNormal3f( 0.0f, 0.5f, 0.0f);
    glVertex3f( size/2.0, sizeY/2.0, -sizeZ/2.0); // Top-right of the quad (Top)
    glVertex3f(-size/2.0, sizeY/2.0, -sizeZ/2.0); // Top-left of the quad (Top)
    glVertex3f(-size/2.0, sizeY/2.0,  sizeZ/2.0); // Bottom-left of the quad (Top)
    glVertex3f( size/2.0, sizeY/2.0,  sizeZ/2.0); // Bottom-right of the quad (Top)

    glNormal3f( 0.0f,-0.5f, 0.0f);
    glVertex3f( size/2.0, -sizeY/2.0,  sizeZ/2.0); // Top-right of the quad (Bottom)
    glVertex3f(-size/2.0, -sizeY/2.0,  sizeZ/2.0); // Top-left of the quad (Bottom)
    glVertex3f(-size/2.0, -sizeY/2.0, -sizeZ/2.0); // Bottom-left of the quad (Bottom)
    glVertex3f( size/2.0, -sizeY/2.0, -sizeZ/2.0); // Bottom-right of the quad (Bottom)

    glNormal3f( 0.0f, 0.0f, 0.5f);
    glVertex3f( size/2.0,  sizeY/2.0, sizeZ/2.0);  // Top-right of the quad (Front)
    glVertex3f(-size/2.0,  sizeY/2.0, sizeZ/2.0);  // Top-left of the quad (Front)
    glVertex3f(-size/2.0, -sizeY/2.0, sizeZ/2.0);  // Bottom-left of the quad (Front)
    glVertex3f( size/2.0, -sizeY/2.0, sizeZ/2.0);  // Bottom-right of the quad (Front)

    glNormal3f( 0.0f, 0.0f,-0.5f);
    glVertex3f( size/2.0, -sizeY/2.0, -sizeZ/2.0); // Bottom-left of the quad (Back)
    glVertex3f(-size/2.0, -sizeY/2.0, -sizeZ/2.0); // Bottom-right of the quad (Back)
    glVertex3f(-size/2.0,  sizeY/2.0, -sizeZ/2.0); // Top-right of the quad (Back)
    glVertex3f( size/2.0,  sizeY/2.0, -sizeZ/2.0); // Top-left of the quad (Back)

    glNormal3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(-size/2.0,  sizeY/2.0,  sizeZ/2.0); // Top-right of the quad (Left)
    glVertex3f(-size/2.0,  sizeY/2.0, -sizeZ/2.0); // Top-left of the quad (Left)
    glVertex3f(-size/2.0, -sizeY/2.0, -sizeZ/2.0); // Bottom-left of the quad (Left)
    glVertex3f(-size/2.0, -sizeY/2.0,  sizeZ/2.0); // Bottom-right of the quad (Left)

    glNormal3f( 0.5f, 0.0f, 0.0f);
    glVertex3f( size/2.0,  sizeY/2.0, -sizeZ/2.0); // Top-right of the quad (Right)
    glVertex3f( size/2.0,  sizeY/2.0,  sizeZ/2.0); // Top-left of the quad (Right)
    glVertex3f( size/2.0, -sizeY/2.0,  sizeZ/2.0); // Bottom-left of the quad (Right)
    glVertex3f( size/2.0, -sizeY/2.0, -sizeZ/2.0); // Bottom-right of the quad (Right)
    glEnd();   // Done drawing the color cube
}

void GLWidget::paintEvent(QPaintEvent */*event*/)
{
    paintGL();

    QPainter painter(this);

    if(*displayTime || drawNameAgent)
    {
        painter.setRenderHint(QPainter::Antialiasing);

        if(*displayTime)
        {
            const QRect bbox(painter.boundingRect(QRect(0, 0, 0, 0), Qt::AlignLeft, *timeString));
            QSize windowSize = size();

            painter.drawText(windowSize.width()-bbox.width()-15,windowSize.height()-bbox.height(),*timeString);
        }
        if(drawNameAgent)
        {
            for(int i = 0; i < nameAgent.tags.size(); i++)
            {
                painter.drawText(10, 15*(i+1), QString("%1\t%2").arg(nameAgent.tags[i], nameAgent.values[i]));
            }
        }
    }

    painter.end();

    glFlush();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    x_last_position = event->x();
    y_last_position = event->y();
    if(pickOn) processSelection(x_last_position, y_last_position);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    // if left button
    if (event->buttons() & Qt::LeftButton)
    {
        if(moveOn)
        {
            xmove -= (float)((x_last_position - event->x())/90.0);
            ymove += (float)((y_last_position - event->y())/90.0);
        }
        else
        {
            xrotate -= (float)((x_last_position - event->x())/2.0);
            yrotate -= (float)((y_last_position - event->y())/2.0);
        }
    }
    else if (event->buttons() & Qt::RightButton)
    {
        if(clippingOn)
        {
            zNear += (float)((y_last_position - event->y())/100.0);
            if(zNear < 0.2) zNear = 0.2;
            resizeGL(windowWidth, windowHeight);
        }
        else
            zmove += (float)((y_last_position - event->y())/100.0);
    }

    x_last_position = event->x();
    y_last_position = event->y();
    update();
}

void GLWidget::wheelEvent(QWheelEvent * event)
{
    zmove += (float)(event->delta()/500.0);
    update();
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
        case Qt::Key_P:
            pickOn = true;
            break;
        case Qt::Key_C:
            clippingOn = true;
            break;
        case Qt::Key_Space:
            moveOn = true;
            break;
        default:
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
        case Qt::Key_P:
            pickOn = false;
            break;
        case Qt::Key_C:
            clippingOn = false;
            break;
        case Qt::Key_Space:
            moveOn = false;
            break;
        default:
            event->ignore();
            break;
    }
}

/* The buffer size for picking objects */
#define BUFSIZE 512
void GLWidget::processSelection(int mx, int my)
{
    GLuint select_buf[BUFSIZE]; //selection buffer
    GLint hits; //number of hits
    GLint  viewport[4];

    glGetIntegerv (GL_VIEWPORT, viewport);

    glSelectBuffer(BUFSIZE, select_buf);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);

    //glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPickMatrix ((GLdouble) mx, (GLdouble) (viewport[3] - my), 5.0, 5.0, viewport);
    gluPerspective( 45.0f, window_ratio, zNear, 20.0f );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();

    glTranslatef(xmove, ymove, zmove);
    glRotatef(yrotate, 1.0f, 0.0f, 0.0f);
    glRotatef(xrotate, 0.0f, 0.0f, 1.0f);

    drawAgents(GL_SELECT);

    glPopMatrix();

    hits = glRenderMode (GL_RENDER);

    drawNameAgent = false;

    int i, j;
    int nitems, zmin, zmax, item;
    int minimumDepth;
    int pickItem;

    //qDebug() << "hits = " << hits;
    int index = 0;
    for (i = 0; i < hits; i++)
    {
        nitems = select_buf[index++];
        zmin = select_buf[index++];
        zmax = select_buf[index++];

        //qDebug() << zmin << zmax;
        for(j = 0; j < nitems; j++)
        {
            item = select_buf[index++];
            //qDebug() << j << item ;
        }

        /* Ignore item=0 as these are the backside of objects if transparent */
        if(item > 0)
        {
            //qDebug() << item << zmin << zmax;

            if(i == 0) { minimumDepth = zmin; pickItem = item; }
            else if(zmin < minimumDepth) { minimumDepth = zmin; pickItem = item; }
        }
    }
    if(hits > 0)
    {
        //qDebug() << "Hit item = " << pickItem;
        Agent * a = nameAgents.value(pickItem);
        nameAgent.tags = QStringList(a->tags);
        nameAgent.values = QStringList(a->values);
        drawNameAgent = true;
        /*for(int i = 0; i < a->tags.size(); i++)
        {
            qDebug() << a->tags[i] << a->values[i];
        }*/

        /* Release of button p is not caught because the focus is given to the agent dialog */
        pickOn = false;
        /* Create dialog */
        //AgentDialog * agentDialog = new AgentDialog(a, this);
        //agentDialog->show();
    }

    resizeGL(windowWidth, windowHeight);
}

#include "glwidget.h"
#include <QOpenGLContext>
#include <QWindow>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent), object(NULL)
{
    qDebug("GLWidget init");
}

void GLWidget::setObject(IDrawable *object_)
{
    qDebug("GLWidget::setObject");
    object = object_;
}

void GLWidget::initializeGL() {
    qDebug("GLWidget::initializeGL");
    // From "grabber" example
    static const GLfloat lightPos[4] = { 5.0f, 5.0f, 10.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    qglClearColor(Qt::black);
    qglColor(Qt::white);

    if (object != NULL) {
        object->init();
    }
}

void GLWidget::resizeGL(int width, int height) {
    qDebug("GLWidget::resizeGL");
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, +1.0, -1.0, 1.0, 5.0, 60.0);
}

void GLWidget::paintGL() {
    qDebug("GLWidget::paintGL");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    if (object != NULL) {
        object->draw();
    }

    glPopMatrix();
}

#include "glwidget.h"
#include <QDebug>
#include "main.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent), object(NULL)
{
    qDebug("GLWidget init");

    // Options defautls:
    setWireframe(false);
    setCullFace(true);
}

void GLWidget::setObject(IDrawable *object_)
{
    qDebug("GLWidget::setObject");
    object = object_;
}


void GLWidget::setWireframe(bool enabled) {
    if (enabled == wireframe) {
        return; // Nothing changed
    }
    wireframe = enabled;
    if (wireframe) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    } else {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
    updateGL();
}

void GLWidget::setCullFace(bool enabled) {
    if (enabled == cullFace) {
        return; // Nothing changed
    }
    cullFace = enabled;
    if (cullFace) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
    updateGL();
}

void GLWidget::initializeGL() {
    qDebug("GLWidget::initializeGL");
    initializeOpenGLFunctions();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    initShaders();

    qglClearColor(Qt::black);
    qglColor(Qt::white);

    if (object != NULL) {
        object->init();
    }
}

void GLWidget::resizeGL(int w, int h) {
    qDebug(QString("GLWidget::resizeGL(%1,%2)").arg(w).arg(h).toLocal8Bit());

    // Set OpenGL viewport to cover whole widget
    glViewport(0, 0, w, h);

    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void GLWidget::paintGL() {
    qDebug("GLWidget::paintGL");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    // Calculate model view transformation
    // TODO: get from Camera object
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.translate(0.0, 0.0, -2.0);
    // Set modelview-projection matrix
    qDebug("  setUniformValue");
    program.setUniformValue("mvp_matrix", projection * matrix);

    if (object != NULL) {
        object->draw(&program);
    }

    glPopMatrix();
}

void GLWidget::initShaders() {
    qDebug("GLWidget::initShaders");

    qDebug("  addShaderFromSourceFile");
    if (!program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl")) {
        qDebug() << program.log();
        // TODO: proper error handling
        exit(1);
    }

    qDebug("  link");
    if (!program.link()) {
        qDebug() << program.log();
        // TODO: proper error handling
        exit(1);
    }

    qDebug("  bind");
    if (!program.link()) {
        qDebug() << program.log();
        // TODO: proper error handling
        exit(1);
    }
}

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include "main.h"

class GLWidget : public QGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    void setObject(IDrawable * object_);

public slots:
    void setWireframe(bool enabled);
    void setCullFace(bool enabled);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    void initShaders();

    IDrawable * object;
    bool wireframe;
    bool cullFace;

    QMatrix4x4 projection;

    QGLShaderProgram program;
};

#endif // GLWIDGET_H

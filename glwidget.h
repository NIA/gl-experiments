#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "main.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    void setObject(IDrawable * object_);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    IDrawable * object;
};

#endif // GLWIDGET_H

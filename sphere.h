#ifndef SPHERE_H
#define SPHERE_H
#include <QOpenGLFunctions>
#include "main.h"

class Sphere : public IDrawable, protected QOpenGLFunctions
{
public:
    Sphere(float radius, Index edgesPerMeridian);
    ~Sphere();

    void init();
    void draw(QGLShaderProgram *program);
private:
    void initGeometry();

    float radius;
    Index edgesPerMeridian;

    Vertex * vertices;
    Index verticesCount;
    Index * indices;
    Index indicesCount;
    GLuint vboIds[2]; // two buffers: for vertices and for indices
};

#endif // SPHERE_H

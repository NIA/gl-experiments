#ifndef MAIN_H
#define MAIN_H
#include <QVector3D>

struct Vertex
{
    Vertex() : position(0, 0, 0), normal(0, 0, 0) {}
    Vertex(QVector3D position_, QVector3D normal_) : position(position_), normal(normal_) {}
    QVector3D position;
    QVector3D normal;
};
typedef unsigned int Index;

class IDrawable {
public:
    virtual void draw() = 0;
};

#endif // MAIN_H

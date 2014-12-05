#include "sphere.h"
//#include <QGLBuffer>
#include <qmath.h>

Sphere::Sphere(float radius_, Index edgesPerMeridian)
    : radius(radius_), edgesPerMeridian(edgesPerMeridian),
      verticesCount(0), indicesCount(0)
{
    qDebug("Sphere init");
}

void Sphere::init() {
    qDebug("Sphere::init");

    qDebug("  initializeOpenGLFunctions()");
    initializeOpenGLFunctions();
    qDebug("  glGenBuffers");
    glGenBuffers(2, vboIds);
    // init geometry
    initGeometry(); // also initializes verticesCount and indicesCount

    qDebug("  data -> buffers");
    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(Index), indices, GL_STATIC_DRAW);
}


void Sphere::draw(QGLShaderProgram *program)
{
    qDebug("Sphere::draw");
    qDebug("  glBindBuffer");
    // Tell OpenGL which VBOs to use
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);

    qDebug("  enableAttributeArray");
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    // Enable wireframe
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // Draw geometry using indices from VBO 1
    qDebug("  glDrawElements");
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

    // Disable wireframe
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}


// Sphere generation code ported from NIA's Crash-And-Squeeze (Renderer)
// https://github.com/NIA/crash-and-squeeze/blob/master/Renderer/sphere.cpp
inline Index sphereVerticesCount(Index edgesPerMeridian)
{
    return 2 // 2 poles
           + (edgesPerMeridian - 1)*2*edgesPerMeridian; // (n - 1) layers of 2*n vertices
}

// Calculated for GL_TRIANGLES mode
inline Index sphereIndicesCount(Index edgesPerMeridian)
{
    return 3*( 4*edgesPerMeridian // 4*n triangles, 2*n near each pole
             + 2*(edgesPerMeridian - 2)*2*edgesPerMeridian ); // 2 triangles per cell, (n - 2) layers of 2*n cells
}

// a helper for filling index buffers
inline void addTriangle( Index i1, Index i2, Index i3, Index *indices, DWORD &currentIndex, Index offset = 0 )
{
    indices[currentIndex++] = i1 + offset;
    indices[currentIndex++] = i2 + offset;
    indices[currentIndex++] = i3 + offset;
}

void Sphere::initGeometry() {
    qDebug("Sphere::initGeometry");
    Q_ASSERT_X(edgesPerMeridian != 0, "Sphere::initGeometry", "edgesPerMeridian == 0");
    verticesCount = sphereVerticesCount(edgesPerMeridian);
    indicesCount = sphereIndicesCount(edgesPerMeridian);
    vertices = new Vertex[verticesCount];
    indices = new Index[indicesCount];

    Index vertex = 0;
    DWORD index = 0;

    float angleStep = static_cast<float>(M_PI)/edgesPerMeridian;

    Index edgesPerDiameter = 2*edgesPerMeridian;

    for(Index thetaIndex = 0; thetaIndex <= edgesPerMeridian; ++thetaIndex) {
        float theta = thetaIndex*angleStep;

        for(Index phiIndex = 0; phiIndex < edgesPerDiameter; ++ phiIndex) {
            float phi = phiIndex*angleStep;

            QVector3D normal(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));

            // add vertex only if not last layer: south pole
            bool addVertex = !(edgesPerMeridian == thetaIndex && phiIndex != 0);

            if(addVertex) {
                vertices[vertex] = Vertex(/*position +*/ radius*normal, normal);
            }

            Index lastVertex = addVertex ? vertex : vertex - 1;
            Index vertexOver = addVertex ? lastVertex - edgesPerDiameter : lastVertex - edgesPerDiameter + phiIndex; // if vertex was not added, vertex counter is not incremented, have to add
            Index vertexBack =     ( 0 != phiIndex ) ? lastVertex - 1 : lastVertex + edgesPerDiameter - 1;
            Index vertexOverBack = ( 0 != phiIndex ) ? vertexOver - 1 : vertexOver + edgesPerDiameter - 1;

            if(1 == thetaIndex) {
                // first layer: connect to north pole (north pole is vertex_over and is 0)
                vertexOver = 0;
                addTriangle(vertexBack, lastVertex, vertexOver, indices, index);
            }
            else if(edgesPerMeridian == thetaIndex)
            {
                // last layer: connect to south pole (south pole is vertex)
                addTriangle(vertexOverBack, lastVertex, vertexOver, indices, index);
            }
            else if(0 != thetaIndex)
            {
                // connect up and back
                addTriangle(vertexBack, lastVertex, vertexOver, indices, index);
                addTriangle(vertexBack, vertexOver, vertexOverBack, indices, index);
            }

            if(addVertex)
            {
                // if vertex really was added, increment counter
                ++vertex;
            }

            if(0 == thetaIndex)
            {
                // north pole is only one point, break phi loop
                break;
            }
        }
    }
}

Sphere::~Sphere() {
    glDeleteBuffers(2, vboIds);
}

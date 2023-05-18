#pragma once

#include <library/Common.h>
#include <QGLShaderProgram>

#include <stdio.h>

typedef vec4  point4;
const int NumVertex = 6;

class FittedPlane
{
public:
    FittedPlane();
    FittedPlane(int an, int al, int profu);
    ~FittedPlane();
    void aplicaTG(mat4 m);
    void read(const QJsonObject &o);
protected:

    void quad( int a, int b, int c, int d );

    int w; /* amplada */
    int h; /* alcada */
    int d; /* profunditat */

    point4 vertex[4]; /* 4 vertexs del pla */

    int Index;
    point4 points[NumVertex];

    GLuint buffer;
};

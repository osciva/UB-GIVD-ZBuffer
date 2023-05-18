#pragma once

#include <library/Common.h>
#include <QGLShaderProgram>


#include <stdio.h>

typedef vec4  point4;
typedef vec4  color4;

const int NumVertex = 6; /* (1 face)(2 triangles/face)(3 vertices/triangle) */

class FittedPlane
{
public:
    FittedPlane();
    FittedPlane(vec3 normal, vec3 point, vec2 pmin, vec2 pmax);
    ~FittedPlane() {}

    vec3 point;
    vec2 pmin;
    vec2 pmax;

    void make();

protected:
    void triangle(int a, int b, int c);

    vec3 normal;


    point4 vertexs[4]; /* 4 vertexs del pla */
    color4 colors[4];

    int Index;
    point4 points[NumVertex];
    color4 vertexs_colors[NumVertex];
};

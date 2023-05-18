#pragma once

#include <library/Common.h>
#include <QGLShaderProgram>

#include <QOpenGLTexture>

#include "GPUConnections/GPUConnectable.hh"
#include "Model/Modelling/Objects/FittedPlane.hh"
#include "GPUConnections/GPUMaterial.hh"

#include <stdio.h>

class GPUFittedPlane : public FittedPlane, public GPUConnectable, public QObject
{
public:
    GPUFittedPlane();
    GPUFittedPlane(int an, int al, int profu, GLfloat x0, GLfloat y0, GLfloat z0);
    ~GPUFittedPlane();
    void make();
    void toGPU(shared_ptr<QGLShaderProgram> program);
    void draw();
    void aplicaTG(mat4 m);
    void setMaterial(shared_ptr<GPUMaterial> m);
private:

    GLdouble xorig, yorig, zorig;

    shared_ptr<GPUMaterial> material;

    GLuint buffer;
};

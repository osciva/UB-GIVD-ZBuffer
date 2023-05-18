#pragma once

#include <library/Common.h>
#include <QGLShaderProgram>

#include <QOpenGLTexture>

#include "GPUConnections/GPUConnectable.hh"
#include "Model/Modelling/Objects/FittedPlane.hh"
#include "GPUConnections/GPUMaterial.hh"
#include "GPUConnections/GPUMaterialFactory.hh"

#include <stdio.h>

class GPUFittedPlane : public FittedPlane, public GPUConnectable, public QObject
{
public:
    GPUFittedPlane();
    GPUFittedPlane(vec3 normal, vec3 point, vec2 pmin, vec2 pmax);
    ~GPUFittedPlane() {}

    void toGPU(shared_ptr<QGLShaderProgram> pr);
    void draw();
    void setMaterial(shared_ptr<GPUMaterial> m);
    void aplicaTG(mat4 m);

    void read(const QJsonObject &json);

    shared_ptr<QGLShaderProgram> program;

private:
    shared_ptr<GPUMaterial> material;

    /* GPU variables */
    GLuint buffer;
    GLuint vao;
};

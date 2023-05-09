#pragma once

#include <library/Common.h>
using namespace Common;

#include <QOpenGLTexture>

#include "GPUConnections/GPUConnectable.hh"
#include "Model/Modelling/Objects/Object.hh"

static int NUMPOINTS = 10000;

class GPUObject : public Object, public GPUConnectable, public QObject
{
public:
    GPUObject();
    GPUObject(const QString &fileName);
    GPUObject(const int npoints, const QString &fileName);

    ~GPUObject();

    virtual void toGPU(shared_ptr<QGLShaderProgram> p) override;
    virtual void draw() override;
    Capsa3D calculCapsa3D();

    void read(const QJsonObject &json) override;
private:
    // Estructures per passar a la GPU
    GLuint buffer;
    GLuint vao;

    // Representació explícita de l'objecte per poder ser passat a la GPU
    int   numPoints;
    vec4 *points;
    vec4 *normals;

    // Els materials s'usen en la primera execució però després són prescindibles
    vec4 *materials;

    int Index; // index de control del numero de vèrtexs a passar a la GPU

    shared_ptr<QOpenGLTexture> texture;

    void make();

    void setTexture(shared_ptr<QOpenGLTexture> t);
    void initTexture();
};


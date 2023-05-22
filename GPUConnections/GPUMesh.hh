#pragma once

#include <library/Common.h>
using namespace Common;

#include <QOpenGLTexture>

#include "GPUConnections/GPUConnectable.hh"
#include "Model/Modelling/Objects/Mesh.hh"
#include "GPUConnections/GPUMaterial.hh"

static int NUMPOINTS = 10000;

class GPUMesh : public Mesh, public GPUConnectable, public QObject
{
public:
	GPUMesh();
    GPUMesh(const int npoints);
    GPUMesh(const QString &fileName);
    GPUMesh(const int npoints, const QString &fileName);

    ~GPUMesh();

    virtual void toGPU(shared_ptr<QGLShaderProgram> p) override;
    void toGPUTexture(shared_ptr<QGLShaderProgram> pr);
    virtual void draw() override;
    Capsa3D calculCapsa3D();

    shared_ptr<QGLShaderProgram> program;

    void read(const QJsonObject &json) override;
    void setMaterial(shared_ptr<GPUMaterial> m);

private:
    // Representació explícita de l'objecte per poder ser passat a la GPU
    int   numPoints;
    vec4 *normals;

    /* Materials */
    shared_ptr<GPUMaterial> material;


    // Els colors s'usen en la primera execució però després són prescindibles
    vec4 *colors;
    vec2 *textures;
    bool hasTexture = false;

    void make();

    void setTexture(shared_ptr<QOpenGLTexture> t);
    void initTextura();

protected:
    // Estructures per passar a la GPU
    GLuint buffer;
    GLuint vao;

    int Index; // index de control del numero de vèrtexs a passar a la GPU
    vec4 *points;

    shared_ptr<QOpenGLTexture> texture;
};


#ifndef GPUCUB_HH
#define GPUCUB_HH

#include "GPUConnections/GPUConnectable.hh"
#include "GPUConnections/GPUMesh.hh"

class GPUCub : public GPUMesh, public GPUConnectable {
public:
    GPUCub();
    GPUCub(float p);

    ~GPUCub() {}

    void toGPU(shared_ptr<QGLShaderProgram> pr);
    void draw();
    void initTextura();

private:
    vector<QImage> faces;
    void initVertex();
    void make();
};

#endif // GPUCUB_HH

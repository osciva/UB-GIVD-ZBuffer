#pragma once

#include "GPUConnections/GPULight.hh"
#include "Model/Modelling/Lights/DirectionalLight.hh"

class GPUDirectionalLight : public GPULight, public DirectionalLight {
public:
    GPUDirectionalLight() {};
    GPUDirectionalLight(vec3 direction, vec3 Ia, vec3 Id, vec3 Is);
    virtual ~GPUDirectionalLight() {}

    virtual void toGPU(shared_ptr<QGLShaderProgram> p) override;
    virtual vec3 vectorL(vec3 point) override;
    virtual float attenuation(vec3 point) override;
    virtual float distanceToLight(vec3 point) override;
};

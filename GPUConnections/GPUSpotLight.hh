#pragma once

#include "GPUConnections/GPULight.hh"
#include "Model/Modelling/Lights/SpotLight.hh"

class GPUSpotLight: public GPULight, public SpotLight {
public:
    GPUSpotLight();
    GPUSpotLight(vec3 posicio, vec3 direction, float opening, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c);
    virtual ~GPUSpotLight() {}

    virtual void toGPU(shared_ptr<QGLShaderProgram> p) override;
    virtual vec3 vectorL(vec3 point) override;
    virtual float attenuation(vec3 point) override;
    virtual float distanceToLight(vec3 point) override;
};

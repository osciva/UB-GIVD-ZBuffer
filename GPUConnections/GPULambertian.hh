#pragma once

#include "GPUConnections/GPUMaterial.hh"
#include "Model/Modelling/Materials/Lambertian.hh"

class GPULambertian: public GPUMaterial, public Lambertian {
public:
    GPULambertian() {}

    GPULambertian(const vec3& color);
    GPULambertian(const vec3& a, const vec3&d, const vec3& s, const float shin);
    GPULambertian(const vec3& a, const vec3&d, const vec3& s, const float shin, const float opacity);

    virtual ~GPULambertian() {}

    virtual void toGPU(shared_ptr<QGLShaderProgram> program) override;
    virtual bool scatter(const Ray& r_in, const HitInfo& rec, vec3& color, Ray & r_out) const;
    virtual vec3 getDiffuse(vec2 uv) const;
};

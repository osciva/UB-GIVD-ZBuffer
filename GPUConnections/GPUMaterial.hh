#pragma once

#include <vector>
#include "Model/Modelling/Ray.hh"
#include "Model/Modelling/Hitable.hh"
#include "DataInOut/Serializable.hh"
#include "GPUConnections/GPUConnectable.hh"
#include "Model/Modelling/Lights/Light.hh"
#include "Model/Modelling/Materials/Material.hh"

class GPUMaterial: public Material, public GPUConnectable
{
public:

    GPUMaterial();
    GPUMaterial(vec3 d);
    GPUMaterial(vec3 a, vec3 d, vec3 s, float shininess);
    GPUMaterial(vec3 a, vec3 d, vec3 s, float shininess, float opacity);
    ~GPUMaterial();

    virtual bool scatter(const Ray& r_in, const HitInfo& rec, vec3& color, Ray & r_out) const override;

    void read (const QJsonObject &json) override;

    virtual void toGPU(shared_ptr<QGLShaderProgram> program) override;
};



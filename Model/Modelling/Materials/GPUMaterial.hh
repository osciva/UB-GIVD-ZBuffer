#pragma once

#include <vector>
#include "Model/Modelling/Ray.hh"
#include "Model/Modelling/Hitable.hh"
#include "DataInOut/Serializable.hh"
#include "GPUConnections/GPUConnectable.hh"
#include "Model/Modelling/Lights/Light.hh"

// Classe abstracte Material. Totes les seves filles hauran de definir el metode abstracte sccater implementat
class GPUMaterial: public Serializable, public Light, public GPUConnectable
{
public:

    GPUMaterial();
    GPUMaterial(vec3 d);
    GPUMaterial(vec3 a, vec3 d, vec3 s, float shininess);
    GPUMaterial(vec3 a, vec3 d, vec3 s, float shininess, float opacity);
    ~GPUMaterial();

    virtual bool scatter(const Ray& r_in, const HitInfo& rec, vec3& color, Ray & r_out) const = 0;
    virtual vec3 getDiffuse(vec2 point) const;

    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 kt;

    float shininess;
    float opacity; // opacity es la fraccio de 0..1 (0 és totalment transparent, 1 és totalment opac)

    virtual void read (const QJsonObject &json);
    virtual void write(QJsonObject &json) const;
    virtual void print(int indentation) const;
    virtual void toGPU(QGLShaderProgram *program);

};



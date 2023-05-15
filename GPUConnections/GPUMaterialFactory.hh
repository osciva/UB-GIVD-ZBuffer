#pragma once

#include "library/Singleton.hh"
#include "GPUConnections/GPUMaterial.hh"
#include "GPUConnections/GPULambertian.hh"
#include "Model/Modelling/Materials/MaterialFactory.hh"

class GPUMaterialFactory: public Singleton<GPUMaterialFactory>
{
    /* Needs to be friend in order to access the private constructor/destructor */
    friend class Singleton<GPUMaterialFactory>;

private:
    GPUMaterialFactory() {}
    ~GPUMaterialFactory() {}

public:
    shared_ptr<GPUMaterial> createMaterial(MaterialFactory::MATERIAL_TYPES t);
    shared_ptr<GPUMaterial> createMaterial(vec3 a, vec3 d, vec3 s, float shin, float opacity, MaterialFactory::MATERIAL_TYPES t);
};

#include "GPUConnections/GPUMaterialFactory.hh"

shared_ptr<GPUMaterial> GPUMaterialFactory::createMaterial(MaterialFactory::MATERIAL_TYPES t) {
    shared_ptr<GPUMaterial> m;

    switch (t) {
    case MaterialFactory::MATERIAL_TYPES::LAMBERTIAN:
        m = make_shared<GPULambertian>();
        break;
    default:
        break;
    }
    return m;
}

shared_ptr<GPUMaterial> GPUMaterialFactory::createMaterial(vec3 a, vec3 d, vec3 s, float shin, float opacity, MaterialFactory::MATERIAL_TYPES t) {
    shared_ptr<GPUMaterial> m;
    switch (t) {
    case MaterialFactory::MATERIAL_TYPES::LAMBERTIAN:
        m = make_shared<GPULambertian>(a, d, s, shin, opacity);
        break;
    default:
        break;
    }
    return m;
}

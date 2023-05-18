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

MaterialFactory::MATERIAL_TYPES GPUMaterialFactory::getIndexType(shared_ptr<GPUMaterial> m) {
    if (dynamic_pointer_cast<GPULambertian>(m) != nullptr) {
        return MaterialFactory::LAMBERTIAN;
    }
    return MaterialFactory::LAMBERTIAN;
}

MaterialFactory::MATERIAL_TYPES GPUMaterialFactory::getMaterialType( QString name) {
    if (name=="LAMBERTIAN") return MaterialFactory::LAMBERTIAN;
    else if (name=="METAL") return MaterialFactory::METAL;
    else if (name=="TRANSPARENT") return MaterialFactory::TRANSPARENT;
    else if (name=="MATERIALTEXTURA") return MaterialFactory::MATERIALTEXTURA;
    else return  MaterialFactory::LAMBERTIAN;
}

QString GPUMaterialFactory::getNameType(MaterialFactory::MATERIAL_TYPES t)
{
    switch(t) {
    case MaterialFactory::LAMBERTIAN:
        return(QString("LAMBERTIAN"));
        break;
    case MaterialFactory::METAL:
        return(QString("METAL"));
        break;
    case MaterialFactory::TRANSPARENT:
        return(QString("TRANSPARENT"));
        break;
    case MaterialFactory::MATERIALTEXTURA:
        return(QString("MATERIALTEXTURA"));
        break;
    default:
        return(QString("LAMBERTIAN"));
        break;
    }
}

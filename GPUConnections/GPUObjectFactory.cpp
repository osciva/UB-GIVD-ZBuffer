#include "GPUObjectFactory.hh"

// TODO Fase 1: Crea objectes de més tipus
// Trobaràs l'enumeració d'OBJECT_TYPES en el fitxer FactoryObject.hh
shared_ptr<Object> GPUObjectFactory::createObject(ObjectFactory::OBJECT_TYPES t)
{
    shared_ptr<Object> o;
    switch (t) {
    case ObjectFactory::OBJECT_TYPES::MESH:
        o = make_shared<GPUMesh>();
        break;
    default:
        break;
    }

    return o;
}

shared_ptr<GPUFittedPlane> GPUObjectFactory::createFitted(ObjectFactory::OBJECT_TYPES t) {
    shared_ptr<GPUFittedPlane> fitted;

    fitted = make_shared<GPUFittedPlane>();

    return fitted;
}

shared_ptr<Object> GPUObjectFactory::createObject( QString s, ObjectFactory::OBJECT_TYPES t) {
    shared_ptr<Object> o;
    switch (t) {
    case ObjectFactory::OBJECT_TYPES::MESH:
        o = make_shared<GPUMesh>(s);
        break;
    default:
        break;
    }

    return o;
}

#include "GPUConnections/GPUPointLight.hh"

GPUPointLight::GPUPointLight() : GPULight(POINT), PointLight() {}

GPUPointLight::GPUPointLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c) : GPULight(POINT, vec4(posicio, 1.0), Ia, Id, Is, vec3(a,b,c)), PointLight(posicio, Ia, Id, Is, a, b, c) {
}

void GPUPointLight::toGPU(shared_ptr<QGLShaderProgram> p) {
    GPULight::toGPU(p);
}

vec3 GPUPointLight::vectorL(vec3 point) {
    return PointLight::vectorL(point);
}

float GPUPointLight::attenuation(vec3 point) {
    return PointLight::attenuation(point);
}

float GPUPointLight::distanceToLight(vec3 point) {
    return PointLight::distanceToLight(point);
}

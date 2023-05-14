#include "GPUConnections/GPULambertian.hh"

GPULambertian::GPULambertian(const vec3 &color) : Lambertian(color) {}

GPULambertian::GPULambertian(const vec3 &a, const vec3 &d, const vec3 &s, const float shin) : Lambertian(a, d, s, shin) {}

GPULambertian::GPULambertian(const vec3 &a, const vec3 &d, const vec3 &s, const float shin, const float opacity) : Lambertian(a, d, s, shin, opacity) {}

void GPULambertian::toGPU(shared_ptr<QGLShaderProgram> program) {
    GPUMaterial::toGPU(program);
}

bool GPULambertian::scatter(const Ray &r_in, const HitInfo &rec, vec3 &color, Ray &r_out) const {
    return Lambertian::scatter(r_in, rec, color, r_out);
}

vec3 GPULambertian::getDiffuse(vec2 uv) const {
    return Lambertian::getDiffuse(uv);
}

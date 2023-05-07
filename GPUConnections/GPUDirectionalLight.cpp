#include "GPUConnections/GPUDirectionalLight.hh"

GPUDirectionalLight::GPUDirectionalLight(vec3 direction, vec3 Ia, vec3 Id, vec3 Is) : GPULight(Ia, Id, Is), DirectionalLight(direction, Ia, Id, Is) {
}

void GPUDirectionalLight::toGPU(shared_ptr<QGLShaderProgram> p) {
    GPULight::toGPU(p);

    struct directional_light_data {
        GLuint direction;
    };

    directional_light_data gl_DirectionalLightData;

    QString indexStr = QString::number(index);
    gl_DirectionalLightData.direction = p->uniformLocation("light[" + indexStr + "].direction");

    glUniform3fv(gl_DirectionalLightData.direction, 1, getDirection());
}

vec3 GPUDirectionalLight::vectorL(vec3 point) {
    return DirectionalLight::vectorL(point);
}

float GPUDirectionalLight::attenuation(vec3 point) {
    return DirectionalLight::attenuation(point);
}

float GPUDirectionalLight::distanceToLight(vec3 point) {
    return DirectionalLight::distanceToLight(point);
}

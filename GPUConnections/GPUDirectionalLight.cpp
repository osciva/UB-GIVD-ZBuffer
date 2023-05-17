#include "GPUConnections/GPUDirectionalLight.hh"

GPUDirectionalLight::GPUDirectionalLight() : DirectionalLight(), GPULight(DIRECTIONAL) {}

GPUDirectionalLight::GPUDirectionalLight(vec3 direction, vec3 Ia, vec3 Id, vec3 Is) : GPULight(DIRECTIONAL, vec4(0.0), Ia, Id, Is, vec3(0.0)), DirectionalLight(direction, Ia, Id, Is) {
}

void GPUDirectionalLight::toGPU(shared_ptr<QGLShaderProgram> p) {
    GPULight::toGPU(p);

    struct directional_light_data {
        GLuint direction;
    };

    directional_light_data gl_DirectionalLightData[6];

    /* Obtenció dels identificadors de la GPU: Suposem 'index' de l'i-èssim element del vector */
    gl_DirectionalLightData[this->index].direction = program->uniformLocation(QString("lights[%1].direction").arg(this->index));

    /* Bind de les zones de memòria que corresponen, element a element de l'array */
    glUniform4fv(gl_DirectionalLightData[this->index].direction, 1, vec4(getDirection(), 1.0));
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

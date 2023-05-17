#include "GPUConnections/GPUSpotLight.hh"

GPUSpotLight::GPUSpotLight() : SpotLight(), GPULight(SPOTLIGHT) {}

GPUSpotLight::GPUSpotLight(vec3 posicio, vec3 direction, float opening, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c) : GPULight(SPOTLIGHT, vec4(posicio, 1.0), Ia, Id, Is, vec3(a,b,c)), SpotLight(posicio, direction, opening, Ia, Id, Is, a, b, c) {
}

void GPUSpotLight::toGPU(shared_ptr<QGLShaderProgram> p) {
    // TO DO: Send the spotlight data to the GPU
    GPULight::toGPU(p);

    /* Declarar un vector d'identificadors de memòria de la GPU */
    struct spot_light_data {
        GLuint direction;
        GLuint opening;
    };

    spot_light_data gl_SpotLightData[6];

    /* Obtenció dels identificadors de la GPU: Suposem 'index' de l'i-èssim element del vector */
    gl_SpotLightData[this->index].direction = program->uniformLocation(QString("lights[%1].direction").arg(this->index));
    gl_SpotLightData[this->index].opening = p->uniformLocation(QString("lights[%1].opening").arg(this->index));

    /* Bind de les zones de memòria que corresponen, element a element de l'array */
    glUniform3fv(gl_SpotLightData[this->index].direction, 1, vec4(getDirection(), 1.0));
    glUniform1f(gl_SpotLightData[this->index].opening, getOpening());
}

vec3 GPUSpotLight::vectorL(vec3 point) {
    return SpotLight::vectorL(point);
}

float GPUSpotLight::attenuation(vec3 point) {
    return SpotLight::attenuation(point);
}

float GPUSpotLight::distanceToLight(vec3 point) {
    return SpotLight::distanceToLight(point);
}

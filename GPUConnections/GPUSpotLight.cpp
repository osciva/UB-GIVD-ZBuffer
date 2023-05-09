#include "GPUConnections/GPUSpotLight.hh"

GPUSpotLight::GPUSpotLight(vec3 posicio, vec3 direction, float opening, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c): SpotLight(posicio, direction, opening, Ia, Id, Is, a, b, c) {
}

void GPUSpotLight::toGPU(shared_ptr<QGLShaderProgram> p) {
    // TO DO: Send the spotlight data to the GPU
    GPULight::toGPU(p);

    /* Declarar un vector d'identificadors de memòria de la GPU */
    struct spot_light_data {
        GLuint position;
        GLuint direction;
        GLuint opening;
        GLuint a;
        GLuint b;
        GLuint c;
    };

    spot_light_data gl_SpotLightData;

    /* Obtenció dels identificadors de la GPU: Suposem 'index' de l'i-èssim element del vector */
    QString indexStr = QString::number(index);
    gl_SpotLightData.position = p->uniformLocation("light[" + indexStr + "].position");
    gl_SpotLightData.direction = p->uniformLocation("light[" + indexStr + "].direction");
    gl_SpotLightData.opening = p->uniformLocation("light[" + indexStr + "].opening");
    gl_SpotLightData.a = p->uniformLocation("light[" + indexStr + "].a");
    gl_SpotLightData.b = p->uniformLocation("light[" + indexStr + "].b");
    gl_SpotLightData.c = p->uniformLocation("light[" + indexStr + "].c");

    /* Bind de les zones de memòria que corresponen, element a element de l'array */
    glUniform3fv(gl_SpotLightData.position, 1, getPos());
    glUniform3fv(gl_SpotLightData.direction, 1, getDirection());
    glUniform1f(gl_SpotLightData.opening, getOpening());
    glUniform1f(gl_SpotLightData.a, getA());
    glUniform1f(gl_SpotLightData.b, getB());
    glUniform1f(gl_SpotLightData.c, getC());
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

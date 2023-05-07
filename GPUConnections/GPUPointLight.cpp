#include "GPUConnections/GPUPointLight.hh"

GPUPointLight::GPUPointLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c): PointLight(posicio, Ia, Id, Is, a, b, c) {
}

void GPUPointLight::toGPU(shared_ptr<QGLShaderProgram> p) {
    // TO DO Pràctica 2: Fase 1: enviar les llums a la GPU
    GPULight::toGPU(p);

    /* Declarar un vector d'identificadors de memòria de la GPU */
    struct point_light_data {
        GLuint position;
        GLuint a;
        GLuint b;
        GLuint c;
    };

    point_light_data gl_PointLightData;

    /* Obtenció dels identificadors de la GPU: Suposem 'index' de l'i-èssim element del vector */
    QString indexStr = QString::number(index);
    gl_PointLightData.position = p->uniformLocation("light[" + indexStr + "].position");
    gl_PointLightData.a = p->uniformLocation("light[" + indexStr + "].a");
    gl_PointLightData.b = p->uniformLocation("light[" + indexStr + "].b");
    gl_PointLightData.c = p->uniformLocation("light[" + indexStr + "].c");

    /* Bind de les zones de memòria que corresponen, element a element de l'array */
    glUniform3fv(gl_PointLightData.position, 1, getPos());
    glUniform1f(gl_PointLightData.a, getA());
    glUniform1f(gl_PointLightData.b, getB());
    glUniform1f(gl_PointLightData.c, getC());
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

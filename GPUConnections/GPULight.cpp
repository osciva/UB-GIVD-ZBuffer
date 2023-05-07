#include "GPUConnections/GPULight.hh"

GPULight::GPULight(vec3 Ia, vec3 Id, vec3 Is): Light(Ia, Id, Is){
}

GPULight::GPULight() {
}

void GPULight::toGPU(shared_ptr<QGLShaderProgram> p) {
    // TO DO Pràctica 2: Fase 1: enviar les propietats de Ia, Id i Is a la GPU
    struct lights_id{
        GLuint ia;
        GLuint id;
        GLuint is;
    };

    lights_id gl_IdLights[3];

    QString lightIndex = QString::number(this->index);
    gl_IdLights[this->index].ia = p->uniformLocation(QString("light[%1].ia").arg(lightIndex));
    gl_IdLights[this->index].id = p->uniformLocation(QString("light[%1].id").arg(lightIndex));
    gl_IdLights[this->index].is = p->uniformLocation(QString("light[%1].is").arg(lightIndex));

    /* Bind del programa shader abans d'establir els valors uniformes */
    p->bind();

    glUniform3fv(gl_IdLights[this->index].ia,1,vec4(this->Ia,1.0));
    glUniform3fv(gl_IdLights[this->index].id,1,vec4(this->Id,1.0));
    glUniform3fv(gl_IdLights[this->index].is,1,vec4(this->Is,1.0));

    /* Release del programa shader després d'establir els valors uniformes */
    p->release();
}

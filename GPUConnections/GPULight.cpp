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

    lights_id gl_IdLights[6];

    QString lightIndex = QString::number(this->index);
    gl_IdLights[this->index].ia = p->uniformLocation(QString("light[%1].ia").arg(lightIndex));
    gl_IdLights[this->index].id = p->uniformLocation(QString("light[%1].id").arg(lightIndex));
    gl_IdLights[this->index].is = p->uniformLocation(QString("light[%1].is").arg(lightIndex));

    glUniform3fv(gl_IdLights[this->index].ia, 1, this->Ia);
    glUniform3fv(gl_IdLights[this->index].id, 1, this->Id);
    glUniform3fv(gl_IdLights[this->index].is, 1, this->Is);
}

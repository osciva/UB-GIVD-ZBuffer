#include "GPUConnections/GPULight.hh"

GPULight::GPULight(LightType t, vec4 position, vec3 Ia, vec3 Id, vec3 Is, vec3 coeficients) : Light(t, position, Ia, Id, Is, coeficients){
}

GPULight::GPULight(LightType t) : Light(t) {
}

void GPULight::toGPU(shared_ptr<QGLShaderProgram> p) {
    // TO DO Pràctica 2: Fase 1: enviar les propietats de Ia, Id i Is a la GPU
    struct lights_id{
        GLuint type;
        GLuint ia;
        GLuint id;
        GLuint is;
        GLuint coeficients;
        GLuint position;
    };

    lights_id gl_IdLights[6];

    gl_IdLights[this->index].type = p->uniformLocation(QString("lights[%1].type").arg(this->index));
    gl_IdLights[this->index].ia = p->uniformLocation(QString("lights[%1].ia").arg(this->index));
    gl_IdLights[this->index].id = p->uniformLocation(QString("lights[%1].id").arg(this->index));
    gl_IdLights[this->index].is = p->uniformLocation(QString("lights[%1].is").arg(this->index));
    gl_IdLights[this->index].coeficients = p->uniformLocation(QString("lights[%1].coeficients").arg(this->index));
    gl_IdLights[this->index].position = p->uniformLocation(QString("lights[%1].position").arg(this->index));

    glUniform1i(gl_IdLights[this->index].type, this->type);
    glUniform3fv(gl_IdLights[this->index].ia, 1, this->Ia);
    glUniform3fv(gl_IdLights[this->index].id, 1, this->Id);
    glUniform3fv(gl_IdLights[this->index].is, 1, this->Is);
    glUniform3fv(gl_IdLights[this->index].coeficients, 1, this->coeficients);
    glUniform4fv(gl_IdLights[this->index].position, 1, this->position);
}

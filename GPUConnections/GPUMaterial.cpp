#include "GPUMaterial.hh"

/* Valors arbitraris. Podem decidir canviar-los */
GPUMaterial::GPUMaterial(): Material(){
}

GPUMaterial::~GPUMaterial()
{}

GPUMaterial::GPUMaterial(vec3 d) : Material(d){
}

GPUMaterial::GPUMaterial(vec3 a, vec3 d, vec3 s, float shin) : Material(a, d, s, shin){
}

GPUMaterial::GPUMaterial(vec3 a, vec3 d, vec3 s, float shin, float opac) : Material(a, d, s, shin, opac){
}

bool GPUMaterial::scatter(const Ray& r_in, const HitInfo& rec, vec3& color, Ray & r_out) const  {
    return false;
}

void GPUMaterial::read (const QJsonObject &json)
{
    Material::read(json);
}

void GPUMaterial::toGPU(shared_ptr<QGLShaderProgram> program){
    struct{
        GLuint kd;
        GLuint ks;
        GLuint ka;
        GLuint shininess;
        GLuint opacity;
    }gl_material;


    gl_material.kd = program->uniformLocation("material.Kd");
    gl_material.ka = program->uniformLocation("material.Ka");
    gl_material.ks = program->uniformLocation("material.Ks");
    gl_material.shininess = program->uniformLocation("material.shininess");
    gl_material.opacity = program->uniformLocation("material.opacity");

    glUniform3fv(gl_material.kd, 1, Kd);
    glUniform3fv(gl_material.ks, 1, Ks);
    glUniform3fv(gl_material.ka, 1, Ka);
    glUniform1f(gl_material.shininess, shininess);
    glUniform1f(gl_material.opacity, opacity);
}

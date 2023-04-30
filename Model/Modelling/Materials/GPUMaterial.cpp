#include "GPUMaterial.hh"

//Valors arbitraris. Podem decKdir canviar-los
GPUMaterial::GPUMaterial(): Ka(1.0f), Kd(1.0f), Ks(1.0f) {
    shininess = 1.0f;
}

GPUMaterial::~GPUMaterial()
{}

GPUMaterial::GPUMaterial(vec3 d) {
    Kd = d;
    //Valors arbitraris. Podem decidir canviar-los.
    Ka = vec3(1.0f, 1.0f, 1.0f);
    Ks = vec3(1.0f, 1.0f, 1.0f);
    shininess = 1.0f;

}

GPUMaterial::GPUMaterial(vec3 a, vec3 d, vec3 s, float shin) {
    //Fase 2
    Ka = a;
    Kd = d;
    Ks = s;
    shininess = shin;
}

GPUMaterial::GPUMaterial(vec3 a, vec3 d, vec3 s, float shin, float opac) {
    //Fase 2
    Ka = a;
    Kd = d;
    Ks = s;
    shininess = shin;
    opacity = opac;
}


vec3 GPUMaterial::getDiffuse(vec2 point) const {
    return Kd;
}

void GPUMaterial::read (const QJsonObject &json)
{
    if (json.contains("ka") && json["ka"].isArray()) {
        QJsonArray auxVec = json["ka"].toArray();
        Ka[0] = auxVec[0].toDouble();
        Ka[1] = auxVec[1].toDouble();
        Ka[2] = auxVec[2].toDouble();
    }
    if (json.contains("kd") && json["kd"].isArray()) {
        QJsonArray auxVec = json["kd"].toArray();
        Kd[0] = auxVec[0].toDouble();
        Kd[1] = auxVec[1].toDouble();
        Kd[2] = auxVec[2].toDouble();
    }
    if (json.contains("ks") && json["ks"].isArray()) {
        QJsonArray auxVec = json["ks"].toArray();
        Ks[0] = auxVec[0].toDouble();
        Ks[1] = auxVec[1].toDouble();
        Ks[2] = auxVec[2].toDouble();
    }
    if (json.contains("shininess") && json["shininess"].isDouble())
        shininess = json["shininess"].toDouble();
    if (json.contains("opacity") && json["opacity"].isDouble())
        opacity = json["opacity"].toDouble();

}


//! [1]
void GPUMaterial::write(QJsonObject &json) const
{
    QJsonArray auxArray;
    auxArray.append(Ka[0]);auxArray.append(Ka[1]);auxArray.append(Ka[2]);
    json["ka"] = auxArray;

    QJsonArray auxArray2;
    auxArray2.append(Kd[0]);auxArray2.append(Kd[1]);auxArray2.append(Kd[2]);
    json["kd"] = auxArray2;

    QJsonArray auxArray3;
    auxArray3.append(Ks[0]);auxArray3.append(Ks[1]);auxArray3.append(Ks[2]);
    json["ks"] = auxArray3;
    json["opacity"] = opacity;
    json["shininess"] = shininess;
}

//! [1]

void GPUMaterial::print(int indentation) const
{
    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << indent << "Ka:\t" << Ka[0] << ", "<< Ka[1] << ", "<< Ka[2] << "\n";
    QTextStream(stdout) << indent << "Kd:\t" << Kd[0] << ", "<< Kd[1] << ", "<< Kd[2] << "\n";
    QTextStream(stdout) << indent << "Ks:\t" << Ks[0] << ", "<< Ks[1] << ", "<< Ks[2] << "\n";
    QTextStream(stdout) << indent << "shininess:\t" << shininess<< "\n";
    QTextStream(stdout) << indent << "opacity:\t" << opacity<< "\n";
}


void GPUMaterial::toGPU(QGLShaderProgram *program){
    struct{
        GLuint kd_id;
        GLuint ks_id;
        GLuint ka_id;
        GLuint shininess_id;
    }components_id;


    components_id.kd_id = program->uniformLocation("material.kd");
    components_id.ks_id = program->uniformLocation("material.ks");
    components_id.ka_id = program->uniformLocation("material.ka");
    components_id.shininess_id = program->uniformLocation("material.shininess");

    glUniform4fv(components_id.kd_id,1,Kd);
    glUniform4fv(components_id.ks_id,1,Ks);
    glUniform4fv(components_id.ka_id,1,Ka);
    glUniform1f(components_id.shininess_id,shininess);
}

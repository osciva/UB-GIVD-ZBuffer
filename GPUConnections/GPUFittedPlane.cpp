#include "GPUFittedPlane.hh"

GPUFittedPlane::GPUFittedPlane() : FittedPlane()
{
    qDebug() << "Estic en el constructor del GPUFittedPlane\n";
    this->material = make_shared<GPUMaterial>();
}

/* Constructora amb tots els parametres */
GPUFittedPlane::GPUFittedPlane(vec3 normal, vec3 point, vec2 pmin, vec2 pmax) : FittedPlane(normal, point, pmin, pmax)
{
    qDebug() << "Estic en el constructor parametritzat del GPUFittedPlane\n";
    this->material = make_shared<GPUMaterial>();
}


/* Carrega del GPUFittedPlane a la GPU */
void GPUFittedPlane::toGPU(shared_ptr<QGLShaderProgram> pr){

    qDebug() << "Passo les dades del GPUFittedPlane a la GPU\n";

    program = pr;

    /* Creació d'un vertex array object */
    glGenVertexArrays(1, &vao );
    glBindVertexArray( vao );

    /* Creacio i inicialitzacio d'un buffer object */
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points),
                  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

    /* Set up vertex arrays */
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,  0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  (void*)(sizeof(points)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* Pintat dels arrays definits del GPUFittedPlane: activació del pipeline de GL */
void GPUFittedPlane::draw()
{
   this->material->toGPU(program);
   glDrawArrays( GL_TRIANGLES, 0, NumVertex );
}

void GPUFittedPlane::aplicaTG(mat4 m)
{
    point4 transformed_points[NumVertex];

    for ( int i = 0; i < NumVertex; ++i ) {
        transformed_points[i] = m * points[i];
    }

    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    /* Actualitzacio del vertex array per a preparar per pintar */
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(transformed_points),
                     transformed_points );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void GPUFittedPlane::setMaterial(shared_ptr<GPUMaterial> m){
    this->material = m;
}

void GPUFittedPlane::read(const QJsonObject &json) {
    if (json.contains("material") && json["material"].isObject()) {
        QJsonObject auxMat = json["material"].toObject();
        if (auxMat.contains("type") && auxMat["type"].isString()) {
            QString tipus = auxMat["type"].toString().toUpper();
            MaterialFactory::MATERIAL_TYPES t = MaterialFactory::getInstance().getMaterialType(tipus);
            material = GPUMaterialFactory::getInstance().createMaterial(t);
            material->read(auxMat);
        }

    }

    /*
    if (json.contains("data") && json["data"].isDouble()) {
        data = json["data"].toDouble();
    }*/

    if (json.contains("point") && json["point"].isArray()) {
        QJsonArray auxVec = json["point"].toArray();
        point[0] = auxVec[0].toDouble();
        point[1] = auxVec[1].toDouble();
        point[2] = auxVec[2].toDouble();
    }

    if (json.contains("normal") && json["normal"].isArray()) {
        QJsonArray auxVec = json["normal"].toArray();
        normal[0] = auxVec[0].toDouble();
        normal[1] = auxVec[1].toDouble();
        normal[2] = auxVec[2].toDouble();
    }

    if (json.contains("pmin") && json["pmin"].isArray()) {
        QJsonArray auxVec = json["pmin"].toArray();
        pmin.x = auxVec[0].toDouble();
        pmin.y = auxVec[1].toDouble();
    }

    if (json.contains("pmax") && json["pmax"].isArray()) {
        QJsonArray auxVec = json["pmax"].toArray();
        pmax.x = auxVec[0].toDouble();
        pmax.y = auxVec[1].toDouble();
    }
}

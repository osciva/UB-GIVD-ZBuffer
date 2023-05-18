#include "GPUFittedPlane.hh"

GPUFittedPlane::GPUFittedPlane() : FittedPlane()
{
    qDebug() << "Estic en el constructor del GPUFittedPlane\n";
    // GPUFittedPlane centrat al 0,0,0 amb dimensió 1 a totes les seves arestes
    xorig = -0.5; yorig = -0.50; zorig = -0.5;
}

// Constructora amb tots els parametres
GPUFittedPlane::GPUFittedPlane(int an, int al, int profu, const GLfloat x0, GLfloat y0, GLfloat z0) : FittedPlane(an, al, profu)
{
    qDebug() << "Estic en el constructor parametritzat del GPUFittedPlane\n";
    xorig = x0;
    yorig = y0;
    zorig = z0;
}

// Destructora
GPUFittedPlane::~GPUFittedPlane()
{
}

// Realitzacio de la geometria del GPUFittedPlane en el vertex array
void GPUFittedPlane::make()
{
    qDebug() << "Estic en el make del GPUFittedPlane\n";
    // generacio de la geometria dels triangles per a visualitzar-lo
    Index = 0;
    quad( 1, 0, 3, 2 );
}

// Carrega del GPUFittedPlane a la GPU
void GPUFittedPlane::toGPU(shared_ptr<QGLShaderProgram> program){

    qDebug() << "Passo les dades del GPUFittedPlane a la GPU\n";

    // Creació d'un vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    /* Creacio i inicialitzacio d'un buffer object */

    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points),
                  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
   /* glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors ); */


    /* set up vertex arrays */

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
   glDrawArrays( GL_TRIANGLES, 0, NumVertex );
}

void GPUFittedPlane::aplicaTG(mat4 m)
{
    point4  transformed_points[NumVertex];

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

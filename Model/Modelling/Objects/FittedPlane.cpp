#include "FittedPlane.hh"

FittedPlane::FittedPlane()
{
    qDebug() << "Estic en el constructor del FittedPlane\n";
    /* FittedPlane centrat al 0,0,0 amb dimensió 1 a totes les seves arestes */
    w = 1.0;
    h = 1.0;
    d = 1.0;

    /* vertex of a unit FittedPlanee centered at origin, sides aligned with axes */
    vertex[0] = point4( -0.5, -0.5,  0.5, 1.0 );
    vertex[1] = point4( -0.5,  0.5,  0.5, 1.0 );
    vertex[2] = point4(  0.5,  0.5,  0.5, 1.0 );
    vertex[3] = point4(  0.5, -0.5,  0.5, 1.0 );
}

// Constructora amb tots els parametres
FittedPlane::FittedPlane(int an, int al, int profu)
{
    qDebug() << "Estic en el constructor parametritzat del FittedPlane\n";
    w = an;
    h = al;
    d = profu;
}

// Destructora
FittedPlane::~FittedPlane()
{
}

// quad generates two triangles for each face and assigns colors
//    to the vertex

void FittedPlane::quad( int a, int b, int c, int d )
{
    points[Index] = vertex[a]; Index++;
    points[Index] = vertex[b]; Index++;
    points[Index] = vertex[c]; Index++;
    points[Index] = vertex[a]; Index++;
    points[Index] = vertex[c]; Index++;
    points[Index] = vertex[d]; Index++;
}

void FittedPlane::read(const QJsonObject &json)
{

}

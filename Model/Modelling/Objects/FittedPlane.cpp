#include "FittedPlane.hh"

FittedPlane::FittedPlane()
{
    qDebug() << "Estic en el constructor del FittedPlane\n";
    /* FittedPlane centrat al 0,0,0 amb dimensió 1 a totes les seves arestes */
    normal = vec3(0.0, 0.0, 1.0);
    point = vec3(0.0, 0.0, 0.0);
    pmin = vec2(-0.5, -0.5);
    pmax = vec2(0.5, 0.5);

    /* Amb dos triangles ja formem un pla */
    vertexs[0] = point4(point.x - pmin.x, point.y - pmin.y, point.z, 1.0);
    vertexs[1] = point4(point.x - pmin.x, point.y + pmax.y, point.z, 1.0);
    vertexs[2] = point4(point.x + pmax.x, point.y - pmin.y, point.z, 1.0);
    vertexs[3] = point4(point.x + pmax.x, point.y + pmax.y, point.z, 1.0);

    // RGBA colors
    colors[0] = color4(1.0, 0.0, 0.0, 1.0);  // red
    colors[1] = color4(0.0, 1.0, 0.0, 1.0);  // green
    colors[2] = color4(0.0, 0.0, 1.0, 1.0);  // blue
    colors[3] = color4(1.0, 1.0, 1.0, 1.0);  // white

    make();
}

/* Constructora amb tots els parametres */
FittedPlane::FittedPlane(vec3 normal, vec3 point, vec2 pmin, vec2 pmax)
{
    qDebug() << "Estic en el constructor parametritzat del FittedPlane\n";
    this->normal = normal;
    this->point = point;
    this->pmin = pmin;
    this->pmax = pmax;

    /* Amb dos triangles ja formem un pla */
    vertexs[0] = point4(point.x - pmin.x, point.y - pmin.y, point.z, 1.0);
    vertexs[1] = point4(point.x - pmin.x, point.y + pmax.y, point.z, 1.0);
    vertexs[2] = point4(point.x + pmax.x, point.y - pmin.y, point.z, 1.0);
    vertexs[3] = point4(point.x + pmax.x, point.y + pmax.y, point.z, 1.0);

    colors[0] = color4(1.0, 0.0, 0.0, 1.0);  // red
    colors[1] = color4(0.0, 1.0, 0.0, 1.0);  // green
    colors[2] = color4(0.0, 0.0, 1.0, 1.0);  // blue
    colors[3] = color4(1.0, 1.0, 1.0, 1.0);  // white

    make();
}


/* Generem els dos triangles */
void FittedPlane::triangle(int a, int b, int c)
{
    vertexs_colors[Index] = colors[a]; points[Index] = vertexs[a]; Index++;
    vertexs_colors[Index] = colors[b]; points[Index] = vertexs[b]; Index++;
    vertexs_colors[Index] = colors[c]; points[Index] = vertexs[c]; Index++;
}

void FittedPlane::make(){
    qDebug() << "Inside FittedPlane make method\n";
    Index = 0;

    triangle(0, 1, 2);
    triangle(2, 1, 3);
}


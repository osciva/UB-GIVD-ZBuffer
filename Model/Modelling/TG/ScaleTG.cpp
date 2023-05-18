#include "ScaleTG.hh"

ScaleTG::ScaleTG(vec3 scale) : scalation(scale)
{
    mat4 result(1.0f);
    result[0][0] = scale.x;
    result[1][1] = scale.y;
    result[2][2] = scale.z;

    matTG = result;
}

ScaleTG::ScaleTG(float esc) {
    mat4 result(1.0f);
    result[0][0] = esc;
    result[1][1] = esc;
    result[2][2] = esc;

    matTG = result;
}

ScaleTG::~ScaleTG()
{

}

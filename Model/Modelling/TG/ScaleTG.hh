#ifndef SCALETG_H
#define SCALETG_H

#include "TG.hh"

class ScaleTG : public TG
{
public:
    ScaleTG(vec3 scale);
    ScaleTG(float esc);
    vec3 scalation;
    float escala;
    virtual ~ScaleTG();
};

#endif // SCALETG_H

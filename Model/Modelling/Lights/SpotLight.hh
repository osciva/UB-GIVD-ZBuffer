#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.hh"

class SpotLight: public Light {
public:
    SpotLight() {};
    SpotLight(vec3 posicio, vec3 direction, float opening, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c);
    virtual ~SpotLight() {}

    vec3 getPos();
    vec3 getDirection();
    float getOpening();

    virtual vec3 vectorL(vec3 point) override;
    virtual float attenuation(vec3 point) override;
    virtual float distanceToLight(vec3 point) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;

    float getA() const { return a; }
    float getB() const { return b; }
    float getC() const { return c; }

private:
    vec3 pos;
    vec3 direction;
    float opening;

    float a;
    float b;
    float c;
};

#endif // SPOTLIGHT_H

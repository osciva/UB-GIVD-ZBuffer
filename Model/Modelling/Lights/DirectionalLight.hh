#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.hh"

class DirectionalLight: public Light {
public:
    DirectionalLight();
    /* En les llums direccionals no necessitem els coeficients d'atenuació
       (constants, lineals i quadràtics), ja que es considera que estan
       infinitament llunyans i la seva intensitat no disminueix amb la distància. */
    DirectionalLight(vec3 direction, vec3 Ia, vec3 Id, vec3 Is);
    virtual ~DirectionalLight() {}

    vec3 getDirection();
    virtual vec3 vectorL(vec3 point) override;
    virtual float attenuation(vec3 point) override;
    virtual float distanceToLight(vec3 point) override;

    virtual void read(const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;

private:
    vec3 direction;
};

#endif // DIRECTIONALLIGHT_H

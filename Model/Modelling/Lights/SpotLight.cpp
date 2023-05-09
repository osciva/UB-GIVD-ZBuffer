#include "SpotLight.hh"

SpotLight::SpotLight(vec3 posicio, vec3 direction, float opening, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c): Light(Ia, Id, Is) {
    this->pos = posicio;
    this->direction = normalize(direction);
    this->opening = opening;
    this->a = a;
    this->b = b;
    this->c = c;
}

vec3 SpotLight::getPos() {
    return this->pos;
}

vec3 SpotLight::getDirection() {
    return this->direction;
}

float SpotLight::getOpening() {
    return this->opening;
}

vec3 SpotLight::vectorL(vec3 point) {
    return normalize(pos - point);
}

float SpotLight::attenuation(vec3 point) {
    vec3 L = vectorL(point);
    float cos_theta = dot(L, -direction);
    if (cos_theta < cos(opening * 0.5f)) {
        return 0.0f;
    }
    if (abs(a)<DBL_EPSILON && abs(b)<DBL_EPSILON && abs(c)<DBL_EPSILON) {
        return 1.0f;
    }
    float d = distance(point, pos);
    return 1.0f/(c*d*d + b*d + a);
}

float SpotLight::distanceToLight(vec3 point) {
    return distance(point, pos);
}

void SpotLight::read (const QJsonObject &json)
{
    Light::read(json);

    if (json.contains("position") && json["position"].isArray()) {
        QJsonArray auxVec = json["position"].toArray();
        pos[0] = auxVec[0].toDouble();
        pos[1] = auxVec[1].toDouble();
        pos[2] = auxVec[2].toDouble();
    }
    if (json.contains("a") && json["a"].isDouble())
        a = json["a"].toDouble();
    if (json.contains("b") && json["b"].isDouble())
        b = json["b"].toDouble();
    if (json.contains("c") && json["c"].isDouble())
        c = json["c"].toDouble();
}

//! [1]
void SpotLight::write(QJsonObject &json) const
{
    Light::write(json);
    QJsonArray auxArray;
    auxArray.append(pos[0]);auxArray.append(pos[1]);auxArray.append(pos[2]);
    json["position"] = auxArray;

    json["a"] = a;
    json["b"] = b;
    json["c"] = c;
}
//! [1]

void SpotLight::print(int indentation) const
{
    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << "Point Light" << "\n";
    QTextStream(stdout) << indent + 2 << "position:\t" << pos[0] << ", "<< pos[1] << ", "<< pos[2] << "\n";
    QTextStream(stdout) << indent + 2 << "a:\t" << a << "\n";
    QTextStream(stdout) << indent + 2<< "b:\t" << b << "\n";
    QTextStream(stdout) << indent + 2 << "c:\t" << c << "\n";

}

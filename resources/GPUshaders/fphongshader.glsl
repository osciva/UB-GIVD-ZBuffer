#version 330

in vec4 fPosition;
in vec4 fNormal;
out vec4 colorOut;

/* Struct light */
struct Light
{
    int type;
    vec4 position;
    vec4 direction;
    float opening;
    vec3 ia;
    vec3 id;
    vec3 is;
    vec3 coeficients;
};

uniform Light lights[1];

/* Global light */
uniform vec3 ambientGlobalLight;

/* Struct material */
struct Material
{
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float shininess;
    float opacity;
};

uniform Material material;

/* Observer position */
uniform vec4 obs;

void main()
{
    /* I = kaIa (ambient) + kdId cos(L, N) (diffuse) + ksIs cos(V, R)^alpha (specular) */
    vec3 Itotal = ambientGlobalLight * material.Ka;
    vec3 Id;
    vec3 Is;
    vec3 Ia;

    vec4 N = fNormal;
    vec4 V = normalize(obs - fPosition);
    vec4 L, R;

    float a, b, c, alpha, dist, attenuation;

    for (int i = 0; i < lights.length(); i++) {
        /* Point light */
        if (lights[i].type == 0) {
            L = normalize(lights[i].position - fPosition);
            dist = length(L);

            /* Store attenuation coeficients */
            a = lights[i].coeficients.x;
            b = lights[i].coeficients.y;
            c = lights[i].coeficients.z;

            /* Attenuation calculation */
            attenuation = 1.0 / (a * dist * dist + b * dist + c);
        }
        /* Directional light (no position, only direction) */
        else if (lights[i].type == 1) {
            L = normalize(-lights[i].direction);
            attenuation = 1.0;
        }

        R = reflect(-L, N);
        alpha = material.shininess;
        Id = lights[i].id * material.Kd * max(dot(N, L), 0.0);
        Is = lights[i].is * material.Ks * pow(max(dot(V, R), 0.0), alpha);
        Ia = lights[i].ia * material.Ka;

        Itotal += (Id + Is) * attenuation + Ia;
    }

    colorOut = vec4(Itotal, 1.0);
}

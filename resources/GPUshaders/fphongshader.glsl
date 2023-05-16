#version 330

in vec4 position;
in vec4 normal;
out vec4 colorOut;
uniform vec3 ambientGlobalLight;
uniform vec4 lookFrom;

/* Struct light */
struct Light
{
    vec4 position;
    vec3 direction;
    float opening;
    vec3 ia;
    vec3 id;
    vec3 is;
    float a;
    float b;
    float c;
};

uniform Light lights[1];


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

void main()
{
    //I = kaIa (ambient) + kdId cos( ⃗ L , ⃗ N ) (difosa) + ksIs cos( ⃗V, ⃗R)α (especular)

    vec3 total = vec3(0,0,0);
    vec3 diffuse, specular, R, L, V, ia, id, is, ka, kd, ks, N, ambient;
    float alpha, factor, dist;

    ka = material.Ka; /* Component ambient */
    kd = material.Kd; /* Component difusa */
    ks = material.Ks; /* Component especualr */
    vec3 ambientGl = ambientGlobalLight * material.Ka;

    V = normalize(lookFrom-position); /* Vector del punt de la superfície a l'observador */
    alpha = material.shininess; /* Exponent de reflexió especular */
    N = normalize(normal); /* Vector de la normal al punt */

    for (int i = 0; i < (int)lights.size(); i++) {
        L = normalize(lights[i].vectorL(position)); /* Vector del punt de la superfície a la llum */
        R = reflect(-L, N); /* R = 2 * N * (dot(N, L)) - L; */
        ia = lights[i].ia; /* Intensitat ambient de la llum */
        id = lights[i].id; /* Intensitat difosa de la llum */
        is = lights[i].is; /* Intensitat especular de la llum */

        diffuse = lights[i].id * kd * glm::max(dot(L,N), 0.0f);
        specular = lights[i].is * ks * pow(glm::max(dot(R, V), 0.0f), alpha);
        ambient = lights[i].ia * ka;

        total += ((diffuse+specular) + ambient); //falta atenuació
    }

    total = total + ambientGl;

    colorOut =  vec4(total,1.0);
}



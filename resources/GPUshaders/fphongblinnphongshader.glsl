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

/* Llum global */
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

/* Posició de l'observador*/
uniform vec4 obs;

void main()
{
    /* I = kaIa (ambient) + kdId cos( ⃗ L , ⃗ N ) (difosa) + ksIs cos( ⃗V, ⃗R)α (especular) */
    vec3 Itotal = ambientGlobalLight * material.Ka;
    vec3 Id;
    vec3 Is;
    vec3 Ia;

    vec4 N = fNormal;
    vec4 V = normalize(obs - fPosition);
    vec4 L, H;

    float a, b, c, dist, attenuation;

    for(int i = 0; i < lights.length(); i++){
        /* Point light */
        if(lights[i].type == 0){
            L = normalize(lights[i].position - fPosition);
            dist = length(L);

            /*  Guardem els coeficients d'atenuació */
            a = lights[i].coeficients.x;
            b = lights[i].coeficients.y;
            c = lights[i].coeficients.z;

            /* Càlcul de l'atenuació */
            attenuation = (a*dist*dist) + (b*dist) + c;
        }

        /* Llum direccional (no tenim posició, només direcció) */
        else if(lights[i].type == 1){
            L = normalize(-lights[i].direction);
            attenuation = 1.0;
        }

        H = normalize(L+V);
        Id = lights[i].id * material.Kd * max(dot(N,L), 0.0);
        Is = lights[i].is * material.Ks * pow(max(dot(N,H), 0.0), material.shininess);
        Ia = lights[i].ia * material.Ka;

        Itotal += (Id + Is)/attenuation + Ia;
    }

    colorOut =  vec4(Itotal,1.0);
}

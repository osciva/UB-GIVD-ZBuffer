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
    vec4 direction;
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
    vec3 Itotal = ambientGlobalLight * material.Ka;
    vec3 idkd;
    vec3 isks;
    vec3 iaka;
    vec4 N = normal;
    vec4 V = normalize(lookFrom - position);
    vec4 L, H;

    vec4 direction;
    float a, b, c, distance, attenuation;

    for(int i=0; i<lights.length(); i++){
        //Point light
        if(lights[i].a != 0){
            L = normalize(lights[i].position - position);
            distance = length(L);
            a = lights[i].a;
            b = lights[i].b;
            c = lights[i].c;
            attenuation = a*distance*distance + b*distance + c;
        }

        //Luz direccional (no tenemos posición, solo dirección)
        else if(lights[i].direction == (0,0,0)){
            L = normalize(-lights[i].direction);
            attenuation = 1.0;
        }
        H = normalize(L+V);
        idkd = lights[i].id * material.Kd  * max(dot(N,L), 0.0);
        isks = lights[i].is * material.Ks * pow(max(dot(N,H), 0.0), material.shininess);
        iaka = lights[i].ia * material.Ka;

        Itotal += ((idkd + isks)/attenuation) + iaka;
    }

    colorOut =  vec4(Itotal,1.0);
}

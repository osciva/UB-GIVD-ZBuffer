#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 normals;

uniform mat4 model_view;
uniform mat4 projection;

out vec4 color;

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
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;

    vec3 Itotal = ambientGlobalLight * material.Ka;
    vec3 idkd;
    vec3 isks;
    vec3 iaka;

    //N es la normal al punto normalizada
    vec4 N = normalize(normals);
    //V es el vector normalizado entre el observador y el punto
    vec4 V = normalize(obs - vPosition);
    //L es el vector normalizado entre la luz y el punto
    //Su calculo varia segun el tipo de luz
    vec4 L;
    //H se calcula a partir de N,L y V
    vec4 H;

    vec4 direction;
    float a, b, c, distance, attenuation;

    //Vamos a ir calculando phong para cada luz segun su tipo {Puntual, Direccional, Spot}
    for(int i=0; i<lights.length(); i++){
        //Luz puntual
        if(lights[i].type == 0){
            L = normalize(lights[i].position - vPosition);
            //Calculamos el valor de la distancia
            distance = length(L);
            //Nos guardamos cada uno de los coeficientes a,b,c
            a = lights[i].coeficients.x;
            b = lights[i].coeficients.y;
            c = lights[i].coeficients.z;
            //Calculo de la atenuación
            attenuation = a*distance*distance + b*distance + c;
        }
        //Luz direccional (no tenemos posición, solo dirección)
        else if(lights[i].type == 1){
            L = normalize(-lights[i].direction);
            attenuation = 1.0;
        }
        H = normalize(L+V);
        idkd = lights[i].id * material.Kd * max(dot(N,L), 0.0);
        isks = lights[i].is * material.Ks * pow(max(dot(N,H), 0.0), material.shininess);
        iaka = lights[i].ia * material.Ka;
        //Para hacer pruebas con los ejemplos del campus hemos quitado la atenuación de la fórmula de phong
        //Itotal += ((idkd + isks)/attenuation) + iaka;
        Itotal += ((idkd + isks)/attenuation) + iaka;
    }
    //El color de salida será el calculado con la fórmula de phong
    color = vec4(Itotal,1.0);
}

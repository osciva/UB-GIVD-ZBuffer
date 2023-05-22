#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 normals;

uniform mat4 model_view;
uniform mat4 projection;

out vec4 fNormal;
out vec4 fPosition;
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

/* Shading model switch */
uniform bool useBlinnPhong;

/* Fornite storm model switch */
uniform bool useForniteStorm;

void main()
{
    gl_Position = projection * model_view * vPosition;
    gl_Position = gl_Position/gl_Position.w;

    fNormal = normals;
    fPosition = vPosition;

    /* Si s'ha activat la Fornite Storm */
    if (useForniteStorm) {
        /* Apliquem Gouraud shading directament amb modalitat phong shading */
        vec3 Itotal = ambientGlobalLight * material.Ka;
        vec3 Id;
        vec3 Is;
        vec3 Ia;

        vec4 N = normalize(normals);
        vec4 V = normalize(obs - vPosition);
        vec4 L;
        vec4 H;
        vec4 R;

        vec4 direction;
        float a, b, c, distance, attenuation;

        for(int i=0; i<lights.length(); i++){
            /* Point light */
            if(lights[i].type == 0){
                L = normalize(lights[i].position - vPosition);
                distance = length(L);

                /* Store attenuation coeficients */
                a = lights[i].coeficients.x;
                b = lights[i].coeficients.y;
                c = lights[i].coeficients.z;

                /* Attenuation calculation */
                attenuation = a*distance*distance + b*distance + c;
            }

            /* Directional light */
            else if(lights[i].type == 1){
                L = normalize(-lights[i].direction);
                attenuation = 1.0;
            }

            H = normalize(L+V);
            Id = lights[i].id * material.Kd * max(dot(N,L), 0.0);

            R = reflect(-L, N);
            Is = lights[i].is * material.Ks * pow(max(dot(V,R), 0.0), material.shininess);

            Ia = lights[i].ia * material.Ka;

            Itotal += ((Id + Is)/attenuation) + Ia;
        }
        /* Apliquem una tonalitat blava per la tempesta */
        Itotal.rgb *= vec3(0.6, 0.6, 1.5);

        /* Enviem el color al fragment shader */
        color = vec4(Itotal,1.0);
    }
}

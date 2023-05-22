#version 330

in vec4 fPosition;
in vec4 fNormal;
in vec4 color;
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

uniform vec2 viewportSize;

uniform mat4 model_view;
uniform mat4 projection;

/* Shading model switch */
uniform bool useBlinnPhong;

/* Night vision model switch */
uniform bool useNightVision;

/* Fornite storm model switch */
uniform bool useForniteStorm;

void main()
{
    /* I = kaIa (ambient) + kdId cos(L, N) (diffuse) + ksIs cos(V, R)^alpha (specular) */
    vec3 Itotal = ambientGlobalLight * material.Ka;
    vec3 Id;
    vec3 Is;
    vec3 Ia;

    vec4 N = fNormal;
    vec4 V = normalize(obs - fPosition);
    vec4 L, R, H;

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

        alpha = material.shininess;
        Id = lights[i].id * material.Kd * max(dot(N, L), 0.0);

        /* Comprovem si s'ha activat BlinnPhong */
        if(useBlinnPhong) {
            H = normalize(L+V);
            Is = lights[i].is * material.Ks * pow(max(dot(N,H), 0.0), alpha);
        } else {
            R = reflect(-L, N);
            Is = lights[i].is * material.Ks * pow(max(dot(V, R), 0.0), alpha);
        }

        Ia = lights[i].ia * material.Ka;
        Itotal += (Id + Is) * attenuation + Ia;
    }

    if (useNightVision) {
        /* Calculem la distància des del gl_FragCoordCalculate distance al centre del viewport */
        vec2 viewportCenter = viewportSize / 2.0;
        float pixelDist = distance(gl_FragCoord.xy, viewportCenter);

        /* Comprovem si està dins del radi */
        if (pixelDist <= viewportSize.y / 4.0) {
            /* Calculem Phong com abans però només considerant el canal verd */
            /* Posem a 0 els canals vermell i blau */
            Itotal.r = 0.0;
            Itotal.b = 0.0;

        } else {
            /* La resta de color negre */
            Itotal = vec3(0.0, 0.0, 0.0);
        }
    } else if (useForniteStorm) {
        /* Transform the 3d coordenates to 2d viewport ones */
        vec4 worldPosition = model_view * fPosition;
        vec4 projectedPosition = projection * worldPosition;

        /* Center of the sphere in projection space */
        vec4 sphereCenter = projection * vec4(0.0, 0.0, 0.0, 1.0);

        /* Distance from the fragment to the center of the sphere in projection space */
        float distanceToSphere = length(projectedPosition - sphereCenter);

        /* Check if the fragment is inside the sphere */
        if (distanceToSphere <= 0.7) {
            /* Apply blue tint to fragments inside the sphere */
            colorOut = color;
        } else {
            colorOut = vec4(Itotal, 1.0);
        }
    } else{
        colorOut = vec4(Itotal, 1.0);
    }
}

#version 330

in vec4 fNormal;
in vec4 fPosition;
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

/* Observer position */
uniform vec4 obs;

void main()
{
    float intensity, silueta;
    vec4 color;

    /* Calculem la direcció de la llum segons el tipus de llum */
    vec4 lightDirection = normalize(lights[0].direction);

    if (lights[0].type == 0) /* Point light */
    {
        /* Suposem llavors que la direcció de la llum és 1.0. */
        lightDirection = vec4(1.0);
    }

    /* Calculem la intensitat del toon entre 4 intervalss */
    intensity = dot(lightDirection, fNormal);

    if (intensity > 0.95)
        color = vec4(1.0, 0.5, 0.5, 1.0);
    else if (intensity > 0.5)
        color = vec4(0.6, 0.3, 0.3, 1.0);
    else if (intensity > 0.25)
        color = vec4(0.4, 0.2, 0.2, 1.0);
    else
        color = vec4(0.2, 0.1 ,0.1, 1.0);

    /* Calculem la silueta amb la fòrmula cos(alfa), on alfa
    és el producte escalar entre la normal i el vector visió. */

    /* Si alfa = 0 tenim una cantonada. */
    silueta = dot(fNormal, normalize(obs - fPosition));

    if (silueta < 0.1)
        color *= silueta;

    colorOut = color;
}


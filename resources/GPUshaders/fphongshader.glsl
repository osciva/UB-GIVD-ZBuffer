#version 330

in vec4 color;
out vec4 colorOut;

struct Material{
    vec3 kd;
    vec3 ks;
    vec3 ka;
    float shininess;
    float opacity;
};

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


void main()
{
    //I = kaIa (ambient) + kdId cos( ⃗ L , ⃗ N ) (difosa) + ksIs cos( ⃗V, ⃗R)α (especular)
    vec3 I = vec3(0,0,0);

    colorOut = vec4((0.7,0.7,0.7),1.0);
}

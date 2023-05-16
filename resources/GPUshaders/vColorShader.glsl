#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 normals;

uniform mat4 model_view;
uniform mat4 projection;

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

out vec4 color;

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;

    color = vec4(material.Kd, 1.0);
}

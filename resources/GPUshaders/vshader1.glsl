#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 normals;

uniform mat4 model_view;
uniform mat4 projection;
uniform vec3 ambientGlobalLight;

out vec4 color;

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
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;

    color = vec4(ambientGlobalLight, 1.0);
}

#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

uniform mat4 model_view;
uniform mat4 projection;

out vec4 color;

struct Material{
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float shin;
};

uniform Material mat;

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;
    color = vec4(mat.kd, 1.0);
}

#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal; // assumeix que les normals estan en el segon atribut de vèrtex

uniform mat4 model_view;
uniform mat4 projection;
uniform mat3 normal_matrix; // aquesta és la matriu de transformació normal

out vec3 fNormal; // aquesta serà la normal que passem al fragment shader

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;

    // transformem la normal i la passem al fragment shader
    fNormal = normal_matrix * vNormal;
}

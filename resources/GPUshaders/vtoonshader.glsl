#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 normals;

uniform mat4 model_view;
uniform mat4 projection;

out vec4 fNormal;
out vec4 fPosition;

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;

    fNormal = normals;
    fPosition = vPosition;
}

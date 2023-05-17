#versio 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vNormal; /* assumeix que les normals estan en el segon atribut de vèrtex */

uniform mat4 model_view;
uniform mat4 projection;

out vec4 fNormal; /* Aquesta serà la normal que passem al fragment shader */

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;

    /* Transformem la normal i la passem al fragment shader */
    fNormal = vNormal;
}

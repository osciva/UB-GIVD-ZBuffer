#version 330

out vec4 colorOut;
float depth;

void main()
{
    depth = gl_FragCoord.z;
    colorOut = vec4(vec3(depth), 1.0);
}


#version 330

in vec3 fNormal; // aquesta és la normal que rebem del vertex shader

out vec4 colorOut;

void main()
{
    // normalitzem la normal per assegurar-nos que té longitud 1
    vec3 normal = normalize(fNormal);

    // convertim la normal de l'espai [-1, 1] a l'espai [0, 1] per usar-la com a color
    vec3 color = normal * 0.5 + 0.5;

    colorOut = vec4(color, 1.0);
}

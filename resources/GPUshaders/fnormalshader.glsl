#version 330

in vec4 fNormal; /* Aquesta és la normal que rebem del vertex shader */

out vec4 colorOut;

void main()
{
    /* Normalitzem la normal per assegurar-nos que té longitud 1 */
    vec4 normal = normalize(fNormal);

    /* Convertim la normal de l'espai [-1, 1] a l'espai [0, 1] per usar-la com a color */
    vec4 color = normal * 0.5 + 0.5;

    colorOut = color;
}

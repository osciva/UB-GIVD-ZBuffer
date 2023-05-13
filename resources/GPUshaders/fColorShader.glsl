#version 330

in vec4 fNormal;
in vec4 fPosition;

uniform vec4 color;

out vec4 colorOut;

void main() {
    colorOut = color;
}

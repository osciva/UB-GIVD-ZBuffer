#version 330

in vec4 fNormal;
in vec4 fPosition;

uniform vec4 material;

out vec4 materialDifus;

void main() {
    materialDifus = material;
}

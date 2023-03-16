#version 330 core

uniform vec3 uColor;
uniform sampler2D uTexture;

in vec2 vFragTexture;
in vec2 vFragPosition;

out vec3 fFragColor;

void main() {
  //fFragColor = uColor;
  fFragColor = texture(uTexture, vFragTexture).xyz;
};


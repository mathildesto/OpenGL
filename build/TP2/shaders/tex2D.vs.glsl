#version 330 core

uniform float uTime;//utilisée pour envoyer le temps écoulé depuis le début de l'application à notre shader.

uniform mat3 uRotateMatrix;
uniform mat3 uScaleMatrix;
uniform mat3 uTranslateMatrix;

layout(location = 3) in vec2 aVertexPosition;
layout(location = 8) in vec2 aVertexTexture;

out vec2 vFragTexture;

out vec2 vFragPosition;

void main() {
  vFragTexture = aVertexTexture;
  //gl_Position = vec4((rotate(uTime)*vec3(aVertexPosition, 1)).xy, 0, 1);
  gl_Position = vec4((uRotateMatrix*uTranslateMatrix*uScaleMatrix*uRotateMatrix*vec3(aVertexPosition, 1)).xy, 0, 1);
  vFragPosition = aVertexPosition;
  
};


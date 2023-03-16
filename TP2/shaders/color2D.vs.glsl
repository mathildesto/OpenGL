#version 330 core

mat3 translate(float tx, float ty){
  mat3 M = mat3(vec3(1, 0, 0), vec3(0, 1, 0), vec3(tx, ty, 1));
  return M;
}

mat3 scale(float sx, float sy){
  mat3 M = mat3(vec3(sx, 0, 0), vec3(0, sy, 0), vec3(0, 0, 1));
  return M;
} 

mat3 rotate(float a){
  float ar = radians(a);
  mat3 M = mat3(vec3(cos(ar), sin(ar), 0), vec3(-(sin(ar)), cos(ar), 0), vec3(0, 0, 1));
  return M;
}

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;

out vec2 vFragPosition;

void main() {
  vFragColor = aVertexColor;
  gl_Position = vec4(aVertexPosition, 0, 1);
  vFragPosition = aVertexPosition;

  //gl_Position = vec4((rotate(45)*translate(0.5, 0.5)*vec3(aVertexPosition, 1)).xy, 0, 1);
  
};

  //translation : 
  //gl_Position = vec4(aVertexPosition + (0.5, 0.5), 0, 1);
  //Avec matrice : 
  //gl_Position = vec4((translate(0.5, 0.5)*vec3(aVertexPosition, 1)).xy, 0, 1);

  //Scale
  //gl_Position = vec4(aVertexPosition*2, 0, 1);
  //gl_Position = vec4(aVertexPosition.x*2, aVertexPosition.y*0.5 , 0, 1);

  //avec matrice :
  //gl_Position = vec4((scale(2, 0.5)*vec3(aVertexPosition, 1)).xy, 0, 1);

  //rotation : 
  //  gl_Position = vec4(rotate(45)*vec3(aVertexPosition, 1)).xy, 0, 1);

  //suite de modif: 
  //gl_Position = vec4((translate(0.5, 0.5)*rotate(45)*scale(2, 0.5)*vec3(aVertexPosition, 1)).xy, 0, 1);
  
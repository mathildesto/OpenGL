#version 330 core

in vec3 vFragColor;
in vec2 vFragPosition;

out vec3 fFragColor;

float attenuation(float a,float b){

  float dis = distance(vFragPosition,vec2(0,0));
  return a*exp(-b*pow(dis,2));

}

float pattern1(){

  return length(fract(10* vFragPosition));

}

float pattern2(){

  return length(abs(fract(5.0 * vFragPosition) * 2.0 - 1.0));

}

float pattern3(){

  return mod(floor(10.0 * vFragPosition.x) + floor(10.0 * vFragPosition.y), 2.0);

}

float pattern4(){

  return smoothstep(0.3, 0.32, length(fract(5.0 * vFragPosition) - 0.5));

}

float pattern5(){

  return smoothstep(0.4, 0.5, max(abs(fract(8.0 * vFragPosition.x - 0.5 * mod(floor(8.0 * vFragPosition.y), 2.0)) - 0.5), abs(fract(8.0 * vFragPosition.y) - 0.5)));

}


void main() {
  //fFragColor = attenuation(1,-3)*vFragColor;

  //fFragColor = pattern5()*vFragColor;

  fFragColor = vFragColor;
};
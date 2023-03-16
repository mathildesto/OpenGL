#version 330

in vec2 vFragPosition;

out vec3 fFragColor;

vec2 complexSqr(vec2 z){
    //calcule le carré du nombre complexe z
    return vec2((z.x)*(z.x) - (z.y)*(z.y), 2.0*(z.x)*(z.y));
}


void main() {

    int Nmax = 500;
    vec2 c = vFragPosition;
    vec2 z = c;
    for (int i = 0; i<Nmax; i++){
        if (length(z) > 2){
            fFragColor = vec3(0.0, 0.0, 0.0);
            break;
        }
        else {
            fFragColor = vec3(1.0, 1.0, 1.0);
            z = complexSqr(z) + c;
        }
    }
}

    //fFragColor = vec3(1.0, 0.0, 0.0) ROUGE 

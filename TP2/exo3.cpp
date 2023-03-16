#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <cstddef>
#include <glimac/Image.hpp>

#include <glimac/glm.hpp>

struct Vertex2DUV {
    Vertex2DUV(glm::vec2 position, glm::vec2 texture) : position(position), texture(texture){};   
    glm::vec2 position;
    glm::vec2 texture;

    Vertex2DUV(){};
 
};

glm::mat3 translate(float tx, float ty){
  glm::mat3 M = glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(tx, ty, 1));
  return M;
}

glm::mat3 scale(float sx, float sy){
  glm::mat3 M = glm::mat3(glm::vec3(sx, 0, 0), glm::vec3(0, sy, 0), glm::vec3(0, 0, 1));
  return M;
} 

glm::mat3 rotate(float a){
  float ar = glm::radians(a);
  glm::mat3 M = glm::mat3(glm::vec3(cos(ar), sin(ar), 0), glm::vec3(-(sin(ar)), cos(ar), 0), glm::vec3(0, 0, 1));
  return M;
}

using namespace glimac;

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    //chargement de l'image
    std::unique_ptr<Image> image = loadImage("/home/mathilde/Documents/GLImac-Template/assets/models/triforce.png");
    if (!image){
        std::cout<<"Failed to load image"<<std::endl;
    }

    //Chargement des shaders
    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/tex2D.vs.glsl",
                              applicationPath.dirPath() + "shaders/tex2D.fs.glsl");
    program.use();

    //GLuint location = glGetUniformLocation(program.getGLId(), "uTime");

    GLuint locationRot = glGetUniformLocation(program.getGLId(), "uRotateMatrix");
    GLuint locationScale = glGetUniformLocation(program.getGLId(), "uScaleMatrix");
    GLuint locationTrans = glGetUniformLocation(program.getGLId(), "uTranslateMatrix");
    GLuint locationColor = glGetUniformLocation(program.getGLId(), "uColor");
    GLuint locationTexture = glGetUniformLocation(program.getGLId(), "uTexture");

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);     
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(),image->getHeight(), 0, GL_RGBA, GL_FLOAT, image->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    Vertex2DUV vertices[] = { 
    Vertex2DUV(glm::vec2(-1.0, -1.0), glm::vec2(0, 1)),
    Vertex2DUV(glm::vec2(1.0, -1.0), glm::vec2(1,1)),
    Vertex2DUV(glm::vec2(0.0, 1.0), glm::vec2(0.5, 0))
};

    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(8);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)(offsetof(Vertex2DUV, position))); //position
    glVertexAttribPointer(8, 2 , GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)(offsetof(Vertex2DUV, texture))); //texture

    // Application loop:
    bool done = false;
    float angle = 0.0;
    while(!done) {
        // Event loop:
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        angle++;
        //glUniform1f (location, angle); 
        glm::mat3 Mrot = rotate(angle);
        glUniformMatrix3fv(locationRot, 1, GL_FALSE, glm::value_ptr(Mrot));

        glm::mat3 Mscale = scale(0.25 , 0.25);
        glUniformMatrix3fv(locationScale, 1, GL_FALSE, glm::value_ptr(Mscale));

        glm::mat3 Mtrans1 = translate(-0.5 , 0.5);
        glUniformMatrix3fv(locationTrans, 1, GL_FALSE, glm::value_ptr(Mtrans1));

        glUniform3f(locationColor, 0.0 , 1.0, 0.0);

        glUniform1i(locationTexture, 0);

        glDrawArrays(GL_TRIANGLES, 0, 3);


        glm::mat3 Mtrans2 = translate(0.5 , -0.5);
        glUniformMatrix3fv(locationTrans, 1, GL_FALSE, glm::value_ptr(Mtrans2));

        glUniform3f(locationColor, 1.0 , 0.0, 0.0);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glm::mat3 Mtrans3 = translate(0.5 , 0.5);
        glUniformMatrix3fv(locationTrans, 1, GL_FALSE, glm::value_ptr(Mtrans3));

        glUniform3f(locationColor, 0.0 , 0.0, 1.0);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glm::mat3 Mtrans4 = translate(-0.5 , -0.5);
        glUniformMatrix3fv(locationTrans, 1, GL_FALSE, glm::value_ptr(Mtrans4));
        glUniform3f(locationColor, 0.5 , 0.1, 0.2);

        glDrawArrays(GL_TRIANGLES, 0, 3);


        // Update the display
        windowManager.swapBuffers();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteTextures(1, &texture);

    return EXIT_SUCCESS;
}

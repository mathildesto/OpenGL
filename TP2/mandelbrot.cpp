#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <cstddef>

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

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/mandelbrot.vs.glsl",
    applicationPath.dirPath() + "shaders/mandelbrot.fs.glsl");

    program.use();

// Création d'un seul VBO:
        GLuint vbo;
        glGenBuffers(1, &vbo);
        // A partir de ce point, la variable vbo contient l'identifiant d'un VBO
        
        // Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // On peut à présent modifier le VBO en passant par la cible GL_ARRAY_BUFFER

        //creer un tableau de GLfloat contenant toutes ces coordonnées à la suite
        GLfloat vertices[] = { -1.f, -1.f, -1.f, 1.f, 1.f, -1.f, -1.f, 1.f, 1.f, -1.f, 1.f, 1.f};
        //Envoi des données
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(vertices), vertices, GL_STATIC_DRAW);

        //Débinder le VBO
        glBindBuffer(GL_ARRAY_BUFFER,0);


        //CREATION D'UN VAO
        GLuint vao;
        glGenVertexArrays(1, &vao);

        //binding
        glBindVertexArray(vao);

        //Activation des attributs de vertex
        const GLuint VERTEX_ATTR_POSITION = 0;
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

        glBindBuffer(GL_ARRAY_BUFFER, vbo); //rebind le VBO

        //Spécification des attributs de vertex
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);


        //debind VAO et VBO
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);


    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        
        //rebinder le VAO
        glBindVertexArray(vao);
        
        //Dessiner en utilisant le VAO
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //Debinder le VAO
        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);


    return EXIT_SUCCESS;
}

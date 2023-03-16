#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <cstddef>
#include<vector>

#include <glimac/glm.hpp>

using namespace glimac;
// ctrl shift p : reload 
// ctrl c


struct Vertex2DColor {
    Vertex2DColor(glm::vec2 pos, glm::vec3 col) : position(pos), color(col){};   
    glm::vec2 position;
    glm::vec3 color;

    Vertex2DColor(){};
 
};

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

    //Chargement des shaders
    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/triangle.vs.glsl",
                              applicationPath.dirPath() + "shaders/triangle.fs.glsl");
    program.use();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    uint32_t N = 8 ;
// Création d'un seul VBO:
        GLuint vbo;
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // => Tableau de sommets : on en a N+1
        std::vector<Vertex2DColor>v{};
        v.push_back(Vertex2DColor(glm::vec2(0.0, 0.0), glm::vec3(0, 1, 0)));
        for (int k = 0; k < N ; k++){
            v.push_back(Vertex2DColor(glm::vec2(glm::cos(k*2*glm::pi<float>()/N), glm::sin(k*2*glm::pi<float>()/N)), glm::vec3(0, 1, 0)));
        }

        glBufferData(GL_ARRAY_BUFFER, (N+1)*sizeof(Vertex2DColor), v.data(), GL_STATIC_DRAW);

        //glBindBuffer(GL_ARRAY_BUFFER,0); ?????????????

        // => Creation du IBO
        GLuint ibo;
        glGenBuffers(1, &ibo);

        // => On bind sur GL_ELEMENT_ARRAY_BUFFER, cible reservée pour les IBOs
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        // => Tableau d'indices: ce sont les indices des sommets à dessiner
        // On en a 3*N afin de former N triangles

        std::vector<uint32_t>indices{};

        for (uint32_t k = 1; k < N+1; k++){
            if (k==N){
                indices.push_back(0);
                indices.push_back(k);
                indices.push_back(1);
            }

            else {
            indices.push_back(0);
            indices.push_back(k);
            indices.push_back(k+1);
            }
        }

        for (int i = 0; i < indices.size(); i++){
        std::cout<< indices[i] <<",";
        }


        // => On remplit l'IBO avec les indices:
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (3*N)* sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

        // => Comme d'habitude on debind avant de passer à autre chose
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //CREATION D'UN VAO
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // => On bind l'IBO sur GL_ELEMENT_ARRAY_BUFFER; puisqu'un VAO est actuellement bindé,
        // cela a pour effet d'enregistrer l'IBO dans le VAO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        const GLuint VERTEX_ATTR_POSITION = 3;
        const GLuint VERTEX_ATTR_COLOR = 8;
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

        glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, position))); //position
        glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, color))); //couleur
        glBindBuffer(GL_ARRAY_BUFFER, 0);
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

        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindVertexArray(vao);
        
        // => On utilise glDrawElements à la place de glDrawArrays
        // Cela indique à OpenGL qu'il doit utiliser l'IBO enregistré dans le VAO
        glDrawElements(GL_TRIANGLES, 3*N, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);


    return EXIT_SUCCESS;
}

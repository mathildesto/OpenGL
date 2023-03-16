#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <cstddef>

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

// -------------------Création d'un seul VBO---------------------------
        GLuint vbo;
        glGenBuffers(1, &vbo);
        // A partir de ce point, la variable vbo contient l'identifiant d'un VBO
        
// -----------Binding d'un VBO sur la cible GL_ARRAY_BUFFER -----------------
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // On peut à présent modifier le VBO en passant par la cible GL_ARRAY_BUFFER

        //creer un tableau de GLfloat contenant toutes ces coordonnées à la suite
        // GLfloat vertices[] = { -0.5f, -0.5f, 1.f, 0.f, 0.f, // premier sommet
        // 0.5f, -0.5f, 0.f, 1.f, 0.f, // deuxième sommet
        // 0.0f, 0.5f, 0.f, 0.f, 1.f // troisième sommet
        // };

        Vertex2DColor vertices[] = { 
        Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(1, 0, 0)),
        Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(0, 1, 0)),
        Vertex2DColor(glm::vec2(-0.5, 0.5), glm::vec3(0, 0, 1)),

        Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(0, 1, 0)),
        Vertex2DColor(glm::vec2(-0.5, 0.5), glm::vec3(0, 0, 1)),
        Vertex2DColor(glm::vec2(0.5, 0.5), glm::vec3(0, 0, 1))
};

        
//-----------------Envoi des données----------------------------------------------------
        //glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW);

        //Débinder le VBO
        //glBindBuffer(GL_ARRAY_BUFFER,0);


//-----------------CREATION D'UN VAO-------------------
        GLuint vao;
        glGenVertexArrays(1, &vao);

//------------------binding----------------------------
        glBindVertexArray(vao);

        //---------Activation des attributs de vertex-------------
        // glEnableVertexAttribArray(GLuint index)
        // GLuint index: l'index de l'attribut à activer
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(8);

    
        //--------Spécification des attributs de vertex------------

        //glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
        //GLuint index: l'index de l'attribut à spécifier
        // GLint size: le nombre de composantes de l'attribut.
        // GLenum type: une constante OpenGL indiquant le type de chaque composante. Dans notre cas on passe GL_FLOAT pour indiquer que chaque composante est un nombre flottant.
        // GLboolean normalized: pas besoin de s'occuper de ce paramètre pour le moment, on passe GL_FALSE
        // GLsizei stride: indique à OpenGL le nombre d'octets séparant l'attribut pour deux sommets consécutifs.
        // const GLvoid* pointer: un pointeur, ou pas. Ce paramètre est un piège. Il faut en fait passer l'offset (décalage) en octets de la premiere instance de l'attribut dans le tableau.

        // glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0); //position
        // glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (const GLvoid*)(2*sizeof(GLfloat))); //couleur

        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, position))); //position
        glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, color))); //couleur

        //debind VAO
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
        
//-------------------rebinder le VAO----------------------
        glBindVertexArray(vao);
        
//-------------Dessiner en utilisant le VAO---------------
        glDrawArrays(GL_TRIANGLES, 0, 6);

//------------------Debinder le VAO-----------------------
        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);


    return EXIT_SUCCESS;
}

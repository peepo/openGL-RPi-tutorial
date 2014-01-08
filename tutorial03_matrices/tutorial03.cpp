#include <stdio.h>
#include <unistd.h>
#include "../common/startScreen.h"
#include "../common/LoadShaders.h"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

int main(int argc, const char **argv)
{
        InitGraphics();
        printf("Screen started\n");
        // Create and compile our GLSL program from the shaders
        GLuint programID = LoadShaders( "simpletransformvertshader.glsl", "simplefragshader.glsl" );
        printf("Shaders loaded\n");

   // Get a handle for our "MVP" uniform
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");

        // Get a handle for our buffers
        GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");

        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        // Or, for an ortho camera :
        //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

        // Camera matrix
        glm::mat4 View       = glm::lookAt(
                                                                glm::vec3(4,3,3), // Camera is at (4,3,3), in Worl$
                                                                glm::vec3(0,0,0), // and looks at the origin
                                                                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to $
                                                   );
        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model      = glm::mat4(1.0f);
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way arou$


        GLfloat g_vertex_buffer_data[] = { 
                -1.0f, -1.0f, 0.0f,
                 1.0f, -1.0f, 0.0f,
                 0.0f,  1.0f, 0.0f,
        };

   // Set the viewport 
//   glViewport ( 0, 0, GScreenWidth, GScreenHeight );
        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        do{

                // Clear the screen
                glClear( GL_COLOR_BUFFER_BIT );

                // Use our shader
                glUseProgram(programID);

                // Send our transformation to the currently bound shader, 
                // in the "MVP" uniform
                glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

                // 1rst attribute buffer : vertices
                glEnableVertexAttribArray(vertexPosition_modelspaceID);
                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
                glVertexAttribPointer(
                        vertexPosition_modelspaceID, // The attribute we want to configure
                        3,                  // size
                        GL_FLOAT,           // type
                        GL_FALSE,           // normalized?
                        0,                  // stride
                        (void*)0            // array buffer offset
                );

// see above glEnableVertexAttribArray(vertexPosition_modelspaceID);
//   glEnableVertexAttribArray ( 0 );

                // Draw the triangle !
                glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

                glDisableVertexAttribArray(vertexPosition_modelspaceID);

        updateScreen();
        }
        while(1);

        // Cleanup VBO
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteProgram(programID);


}


#include <stdio.h>
#include <unistd.h>
#include "../common/startScreen.h"
#include "../common/LoadShaders.h"

int main(int argc, const char **argv)
{
        InitGraphics();
        printf("Screen started\n");
        // Create and compile our GLSL program from the shaders
        GLuint programID = LoadShaders( "simplevertshader.glsl", "simplefragshader.glsl" );
        printf("Shaders loaded\n");

        // Get a handle for our buffers
        GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");

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

}


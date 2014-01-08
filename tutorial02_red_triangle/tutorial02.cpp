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

        GLfloat g_vertex_buffer_data[] = { 
                -1.0f, -1.0f, 0.0f,
                 1.0f, -1.0f, 0.0f,
                 0.0f,  1.0f, 0.0f,
        };

   // Set the viewport 
//   glViewport ( 0, 0, GScreenWidth, GScreenHeight );

        do{

                // Clear the screen
                glClear( GL_COLOR_BUFFER_BIT );

                // Use our shader
                glUseProgram(programID);

                // 1rst attribute buffer : vertices
//                glEnableVertexAttribArray(vertexPosition_modelspaceID);
//                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
                glVertexAttribPointer(
                        0, //vertexPosition_modelspaceID, // The attribute we want to configure
                        3,                  // size
                        GL_FLOAT,           // type
                        GL_FALSE,           // normalized?
                        0,                  // stride
                        g_vertex_buffer_data // (void*)0            // array buffer offset
                );

// see above glEnableVertexAttribArray(vertexPosition_modelspaceID);
   glEnableVertexAttribArray ( 0 );

                // Draw the triangle !
                glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

uint32_t GScreenWidth = 1920;
uint32_t GScreenHeight = 1080;

        void* image = malloc(GScreenWidth*GScreenHeight*4);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glReadPixels(0,0,GScreenWidth,GScreenHeight, GL_RGBA, GL_UNSIGNED_BYTE, image); //GScreenWidth,GScreenHeight,

        updateScreen();
        }
        while(1);

}


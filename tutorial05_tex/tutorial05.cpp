#include <stdio.h>
#include <unistd.h>
#include "../common/startScreen.h"
#include "../common/LoadShaders.h"
#include "../common/texture.h"

int main(int argc, const char **argv)
{
        InitGraphics();
        printf("Screen started\n");
        // Create and compile our GLSL program from the shaders
        GLuint programID = LoadShaders( "simplevertshader.glsl", "texturefragshader.glsl" );
        printf("Shaders loaded\n");

        // Load the texture using any two methods
        GLuint Texture = loadBMP_custom("uvtemplate.bmp");
        //GLuint Texture =  loadDDS("uvtemplate.DDS"); //loadTGA_glfw("uvtemplate.tga"); 

        // Get a handle for our "myTextureSampler" uniform
        GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");


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

                // Bind our texture in Texture Unit 0
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, Texture);
                // Set our "myTextureSampler" sampler to user Texture Unit 0
                glUniform1i(TextureID, 0);

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


        updateScreen();
        }
        while(1);

        // Cleanup VBO
//      glDeleteBuffers(1, &vertexbuffer);
        glDeleteProgram(programID);
        glDeleteTextures(1, &TextureID);
}


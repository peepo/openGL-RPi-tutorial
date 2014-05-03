// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
using namespace glm;

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "../common/startScreen.h"
#include "../common/LoadShaders.h"
#include "../common/texture.h"
#include "../common/objloader.h"

int main( void )
{
InitGraphics();
    printf("Screen started\n");

// Dark blue background
// glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

// Enable depth test
glEnable(GL_DEPTH_TEST);
// Accept fragment if it closer to the camera than the former one
glDepthFunc(GL_LESS);

// Cull triangles which normal is not towards the camera
glEnable(GL_CULL_FACE);

// Create and compile our GLSL program from the shaders
GLuint programID = LoadShaders( "TransformVertexShader.glsl", "TextureFragmentShader.glsl" );

// Get a handle for our "MVP" uniform
GLuint MatrixID = glGetUniformLocation(programID, "MVP");
GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

// Get a handle for our buffers
GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");
GLuint vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    // Camera matrix
    glm::mat4 View = glm::lookAt(
                                glm::vec3(4,1,3), // Camera is at (4,1,3), in World Space
                                glm::vec3(0,0,0), // and looks at the origin
                                glm::vec3(0,1,0) // Head is up (set to 0,-1,0 to look upside-down)
                           );
    
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);

    // Transform for Model
    glm::vec3 position = glm::vec3(0,0,0);
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 scale = glm::vec3(1,1,1);

// Load the texture
GLuint Texture = loadBMP_custom("uvtemplate.bmp");

// Get a handle for our "myTextureSampler" uniform
GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

// Read our .obj file
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;
bool res = loadOBJ("suzanne.obj", vertices, uvs, normals);

// Load it into a VBO
GLuint vertexbuffer;
glGenBuffers(1, &vertexbuffer);
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

GLuint uvbuffer;
glGenBuffers(1, &uvbuffer);
glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

GLuint normalbuffer;
glGenBuffers(1, &normalbuffer);
glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

// Get a handle for our "LightPosition" uniform
glUseProgram(programID);
GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

do{
// Clear the screen
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Use our shader
glUseProgram(programID);
        
        // Rebuild the Model matrix
        rotation.y += 0.01f;
glm::mat4 translationMatrix	= glm::translate(glm::mat4(1.0f), position);
glm::mat4 rotationMatrix	= glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
glm::mat4 scalingMatrix	= glm::scale(glm::mat4(1.0f), scale);

Model = translationMatrix * rotationMatrix * scalingMatrix;

        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

// Send our transformation to the currently bound shader,
// in the "MVP" uniform
glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);

glm::vec3 lightPos = glm::vec3(4,4,4);
glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

// Bind our texture in Texture Unit 0
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, Texture);
// Set our "myTextureSampler" sampler to user Texture Unit 0
glUniform1i(TextureID, 0);

// 1rst attribute buffer : vertices
glEnableVertexAttribArray(vertexPosition_modelspaceID);
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
glVertexAttribPointer(
vertexPosition_modelspaceID, // The attribute we want to configure
3, // size
GL_FLOAT, // type
GL_FALSE, // normalized?
0, // stride
(void*)0 // array buffer offset
);

// 2nd attribute buffer : UVs
glEnableVertexAttribArray(vertexUVID);
glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
glVertexAttribPointer(
vertexUVID, // The attribute we want to configure
2, // size : U+V => 2
GL_FLOAT, // type
GL_FALSE, // normalized?
0, // stride
(void*)0 // array buffer offset
);

// 3rd attribute buffer : normals
glEnableVertexAttribArray(vertexNormal_modelspaceID);
glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
glVertexAttribPointer(
vertexNormal_modelspaceID, // The attribute we want to configure
3, // size
GL_FLOAT, // type
GL_FALSE, // normalized?
0, // stride
(void*)0 // array buffer offset
);

// Draw the triangles !
glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

glDisableVertexAttribArray(vertexPosition_modelspaceID);
glDisableVertexAttribArray(vertexUVID);
glDisableVertexAttribArray(vertexNormal_modelspaceID);

updateScreen();
}
while( 1 );

// Cleanup VBO and shader
glDeleteBuffers(1, &vertexbuffer);
glDeleteBuffers(1, &uvbuffer);
glDeleteBuffers(1, &normalbuffer);
glDeleteProgram(programID);
glDeleteTextures(1, &Texture);

return 0;
}


#include <stdio.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include <assert.h>

#include "LoadShaders.h"

#define check() assert(glGetError() == 0)

GfxShader GSimpleVS;
GfxShader GSimpleFS;
GfxProgram GSimpleProg;

//void LoadShaders(){
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
        GSimpleVS.LoadVertexShader(vertex_file_path); //"simplevertshader.glsl");
        GSimpleFS.LoadFragmentShader(fragment_file_path); //"simplefragshader.glsl");
        GSimpleProg.Create(&GSimpleVS,&GSimpleFS);
        check();
        glUseProgram(GSimpleProg.GetId());
        check();
        return GSimpleProg.GetId();
}

bool GfxShader::LoadFragmentShader(const char* filename)
{
        //cheeky bit of code to read the whole file into memory
        assert(!Src);
        FILE* f = fopen(filename, "rb");
        assert(f);
        fseek(f,0,SEEK_END);
        int sz = ftell(f);
        fseek(f,0,SEEK_SET);
        Src = new GLchar[sz+1];
        fread(Src,1,sz,f);
        Src[sz] = 0; //null terminate it!
        fclose(f);

        //now create and compile the shader
//      GlShaderType = GL_FRAGMENT_SHADER;
        Id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(Id, 1, (const GLchar**)&Src, 0);
        glCompileShader(Id);
        check();

        //compilation check
//      GLint compiled;

        return true;
}

bool GfxShader::LoadVertexShader(const char* filename)
{

        //cheeky bit of code to read the whole file into memory
        assert(!Src);
        FILE* f = fopen(filename, "rb");
        assert(f);
        fseek(f,0,SEEK_END);
        int sz = ftell(f);
        fseek(f,0,SEEK_SET);
        Src = new GLchar[sz+1];
        fread(Src,1,sz,f);
        Src[sz] = 0; //null terminate it!
        fclose(f);

        //now create and compile the shader
        GlShaderType = GL_VERTEX_SHADER;
        Id = glCreateShader(GlShaderType);
        glShaderSource(Id, 1, (const GLchar**)&Src, 0);
        glCompileShader(Id);
        check();

        return true;
}

bool GfxProgram::Create(GfxShader* vertex_shader, GfxShader* fragment_shader)
{
        VertexShader = vertex_shader;
        FragmentShader = fragment_shader;
        Id = glCreateProgram();
        glAttachShader(Id, VertexShader->GetId());
        glAttachShader(Id, FragmentShader->GetId());
        glLinkProgram(Id);
        check();

        return true;    
}



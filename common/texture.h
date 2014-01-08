#pragma once

#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

// Load a .TGA file using GLFW's own loader
// openGLES 2.0 on RPi without windows
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);


#endif

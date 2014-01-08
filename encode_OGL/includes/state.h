#ifndef __STATE_H
#define __STATE_H

#include "GLES/gl.h"
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "matrix.h"

typedef struct
{
   uint32_t screen_width;
   uint32_t screen_height;
// OpenGL|ES objects
   EGLDisplay display;
   EGLSurface surface;
   EGLContext context;
   GLuint tex;
   GLuint render_texture;
	 GLuint offscreen_renderbuffer;
	 GLuint y_framebuffer;
	 GLuint u_framebuffer;
	 GLuint v_framebuffer;
	 GLuint render_program;
	 GLuint y_program;
	 GLuint u_program;
	 GLuint v_program;

	 GLuint attr_vertex;
	 GLuint attr_tex;
	 GLuint unif_tex;
	 GLuint y_tex;
	 GLuint u_tex;
	 GLuint v_tex;
	 GLuint unif_pmatrix;
	 GLuint y_pmatrix;
	 GLuint u_pmatrix;
	 GLuint v_pmatrix;
	 GLuint unif_mvmatrix;
	 GLuint y_mvmatrix;
	 GLuint u_mvmatrix;
	 GLuint v_mvmatrix;

	 void *eglImage;

	 GLuint out_buffers[3];

	 GLuint dump_frame;

	 struct matrix p_matrix;
	 struct matrix mv_matrix;
	 struct matrix send_mv_matrix;

	 char *write_buffer;

	 float roll;
} CUBE_STATE_T;

#endif

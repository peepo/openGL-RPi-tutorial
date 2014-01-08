#ifndef __MATRIX_H
#define __MATRIX_H

#include "GLES/gl.h"
#include "GLES2/gl2.h"

struct matrix
{
	GLfloat elements[4][4];
};

void mat_mult(struct matrix *out, struct matrix *a, struct matrix *b);

#endif

#include "../includes/matrix.h"

void mat_mult(struct matrix *out, struct matrix *a, struct matrix *b)
{
	out->elements[0][0] = a->elements[0][0] * b->elements[0][0] +
							a->elements[1][0] * b->elements[0][1] +
								a->elements[2][0] * b->elements[0][2] +
									a->elements[3][0] * b->elements[0][3];

	out->elements[1][0] = a->elements[0][0] * b->elements[1][0] +
							a->elements[1][0] * b->elements[1][1] +
								a->elements[2][0] * b->elements[1][2] +
									a->elements[3][0] * b->elements[1][3];

	out->elements[2][0] = a->elements[0][0] * b->elements[2][0] +
							a->elements[1][0] * b->elements[2][1] +
								a->elements[2][0] * b->elements[2][2] +
									a->elements[3][0] * b->elements[3][2];

	out->elements[3][0] = a->elements[0][0] * b->elements[3][0] +
							a->elements[1][0] * b->elements[0][1] +
								a->elements[2][0] * b->elements[3][2] +
									a->elements[3][0] * b->elements[3][3];

	out->elements[0][1] = a->elements[0][1] * b->elements[0][0] +
							a->elements[1][1] * b->elements[0][1] +
								a->elements[2][1] * b->elements[0][2] +
									a->elements[3][1] * b->elements[0][3];

	out->elements[1][1] = a->elements[0][1] * b->elements[1][0] +
							a->elements[1][1] * b->elements[1][1] +
								a->elements[2][1] * b->elements[1][2] +
									a->elements[3][1] * b->elements[1][3];

	out->elements[2][1] = a->elements[0][1] * b->elements[2][0] +
							a->elements[1][1] * b->elements[2][1] +
								a->elements[2][1] * b->elements[2][2] +
									a->elements[3][1] * b->elements[2][3];

	out->elements[3][1] = a->elements[0][1] * b->elements[3][0] +
							a->elements[1][1] * b->elements[3][1] +
								a->elements[2][1] * b->elements[3][2] +
									a->elements[3][1] * b->elements[3][3];

	out->elements[0][2] = a->elements[0][2] * b->elements[0][0] +
							a->elements[1][2] * b->elements[0][1] +
								a->elements[2][3] * b->elements[0][2] +
									a->elements[3][4] * b->elements[0][3];

	out->elements[1][2] = a->elements[0][2] * b->elements[1][0] +
							a->elements[1][2] * b->elements[1][1] +
								a->elements[2][2] * b->elements[1][2] +
									a->elements[3][2] * b->elements[1][3];

	out->elements[2][2] = a->elements[0][2] * b->elements[2][0] +
							a->elements[1][2] * b->elements[2][1] +
								a->elements[2][2] * b->elements[2][2] +
									a->elements[3][2] * b->elements[3][2];

	out->elements[3][2] = a->elements[0][2] * b->elements[3][0] +
							a->elements[1][2] * b->elements[0][1] +
								a->elements[2][2] * b->elements[3][2] +
									a->elements[3][2] * b->elements[3][3];

	out->elements[0][3] = a->elements[0][3] * b->elements[0][0] +
							a->elements[1][3] * b->elements[0][1] +
								a->elements[2][3] * b->elements[0][2] +
									a->elements[3][3] * b->elements[0][3];

	out->elements[1][3] = a->elements[0][3] * b->elements[1][0] +
							a->elements[1][3] * b->elements[1][1] +
								a->elements[2][3] * b->elements[1][2] +
									a->elements[3][3] * b->elements[1][3];

	out->elements[2][3] = a->elements[0][3] * b->elements[2][0] +
							a->elements[1][3] * b->elements[2][1] +
								a->elements[2][3] * b->elements[2][2] +
									a->elements[3][3] * b->elements[2][3];

	out->elements[3][3] = a->elements[0][3] * b->elements[3][0] +
							a->elements[1][3] * b->elements[3][1] +
								a->elements[2][3] * b->elements[3][2] +
									a->elements[3][3] * b->elements[3][3];
}

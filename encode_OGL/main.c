#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>

#include "includes/ogl.h"

CUBE_STATE_T state, *p_state = &state;

int encode_loop(CUBE_STATE_T *state);

int main(int argc, char** argv)
{
	init_ogl(p_state);

	create_perspective_matrix(&p_state->p_matrix, 45, 1.0, 0.1, 100);

	identity(&p_state->mv_matrix);
	identity(&p_state->send_mv_matrix);

	rotate_matrix(&p_state->send_mv_matrix, 180, 1.0, 0, 0);
	translate_matrix(&p_state->send_mv_matrix, 0, 0, -5);
	//rotate_matrix(&p_state->mv_matrix, 45, 1.0, 0, 0);
	translate_matrix(&p_state->mv_matrix, 0, 0, -5);

	init_shaders(p_state);

	init_framebuffer(p_state);

	init_textures(p_state);

	encode_loop(p_state);

        while(1)
        {
        }

//	return NULL;
}

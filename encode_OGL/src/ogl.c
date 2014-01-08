#include "../includes/state.h"
#include "../includes/ogl.h"
#include "../includes/matrix.h"
#include "bcm_host.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

void* video_decode_test(void* arg);

// Spatial coordinates for the cube

static const GLfloat quadx[4*3] = {
   /* FRONT */
   -2.07, -2.07,  0,
   2.07, -2.07,  0,
   -2.07,  2.07, 0,
   2.07,  2.07,  0,
};

/** Texture coordinates for the quad. */
static const GLfloat texCoords[4 * 2] = {
   0.f,  0.f,
   1.f,  0.f,
   0.f,  1.f,
   1.f,  1.f,
};


static pthread_t thread1;

void init_ogl(CUBE_STATE_T *state)
{
	int32_t success = 0;
	EGLBoolean result;
	EGLint num_config;

	bcm_host_init();

	static EGL_DISPMANX_WINDOW_T nativewindow;

	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T dispman_update;
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;

	static const EGLint attribute_list[] =
	{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};

	static const EGLint context_attributes[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	EGLConfig config;

	// get an EGL display connection
	state->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	// initialize the EGL display connection
	result = eglInitialize(state->display, NULL, NULL);

	// get an appropriate EGL frame buffer configuration
	result = eglChooseConfig(state->display, attribute_list, &config, 1, &num_config);
	assert(EGL_FALSE != result);

	// get an appropriate EGL frame buffer configuration
	result = eglBindAPI(EGL_OPENGL_ES_API);
	assert(EGL_FALSE != result);


	// create an EGL rendering context
	state->context = eglCreateContext(state->display, config, EGL_NO_CONTEXT, context_attributes);
	assert(state->context!=EGL_NO_CONTEXT);

	// create an EGL window surface
	success = graphics_get_display_size(0 /* LCD */, &state->screen_width, &state->screen_height);
	assert( success >= 0 );

	state->screen_width = 1024;

	state->screen_height = 1024;

	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.width = state->screen_width;
	dst_rect.height = state->screen_height;

	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = state->screen_width << 16;
	src_rect.height = state->screen_height << 16;        

	dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
	dispman_update = vc_dispmanx_update_start( 0 );

	dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
			0/*layer*/, &dst_rect, 0/*src*/,
			&src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/, 0/*transform*/);

	nativewindow.element = dispman_element;
	nativewindow.width = state->screen_width;
	nativewindow.height = state->screen_height;
	vc_dispmanx_update_submit_sync( dispman_update );

	state->surface = eglCreateWindowSurface( state->display, config, &nativewindow, NULL );
	assert(state->surface != EGL_NO_SURFACE);

	// connect the context to the surface
	result = eglMakeCurrent(state->display, state->surface, state->surface, state->context);
	assert(EGL_FALSE != result);

	// Set background color and clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Enable back face culling.
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//   glMatrixMode(GL_MODELVIEW);

	glViewport(0, 0, (GLsizei)state->screen_width, (GLsizei)state->screen_height);

}

void init_shaders(CUBE_STATE_T *state)
{
	const GLchar *vShaderStr = 
		"attribute vec3 vPosition;\n"
		"attribute vec2 TexCoordIn;\n"
		"varying vec2 TexCoordOut;\n"
		"uniform mat4 umvmatrix;\n"
		"uniform mat4 upmatrix;\n"
		"void main() \n"
		"{ \n"
		"	TexCoordOut = TexCoordIn;\n"
		" gl_Position = upmatrix * umvmatrix * vec4(vPosition, 1.0); \n"
		"} \n";

	const GLchar *fShaderStr = 
		"precision mediump float; \n"
		"varying vec2 TexCoordOut;\n"
		"uniform sampler2D texture;\n"
		"void main() \n"
		"{ \n"
		" vec4 tex = texture2D(texture, TexCoordOut);\n"
		" gl_FragColor = tex;\n"
		"}															\n";

	const GLchar *fShaderStrY = 
		"precision highp float; \n"
		"varying vec2 TexCoordOut;\n"
		"uniform sampler2D texture;\n"
		"vec4 conversion_factor = vec4(0.257, 0.504, .098, 0.0625);\n"
		"void main() \n"
		"{ \n"
		" vec4 tex1 = texture2D(texture, TexCoordOut - vec2(.00234375, 0));\n"
		" vec4 tex2 = texture2D(texture, TexCoordOut - vec2(.00078125, 0));\n"
		" vec4 tex3 = texture2D(texture, TexCoordOut + vec2(.00078123, 0));\n"
		" vec4 tex4 = texture2D(texture, TexCoordOut + vec2(.00234375, 0));\n"
		" float Y1 = dot(conversion_factor, vec4(tex1.rgb, 1.0));\n"
		" float Y2 = dot(conversion_factor, vec4(tex2.rgb, 1.0));\n"
		" float Y3 = dot(conversion_factor, vec4(tex3.rgb, 1.0));\n"
		" float Y4 = dot(conversion_factor, vec4(tex4.rgb, 1.0));\n"
		" gl_FragColor = vec4(Y1, Y2, Y3, Y4);\n"
		"}															\n";

	const GLchar *fShaderStrU = 
		"precision highp float; \n"
		"varying vec2 TexCoordOut;\n"
		"uniform sampler2D texture;\n"
		"vec4 conversion_factor = vec4(-0.148, -0.291, .439, 0.5);\n"
		"void main() \n"
		"{ \n"
		" vec4 tex1 = texture2D(texture, TexCoordOut - vec2(.0046875, 0));\n"
		" vec4 tex2 = texture2D(texture, TexCoordOut - vec2(.0015625, 0));\n"
		" vec4 tex3 = texture2D(texture, TexCoordOut + vec2(.0015625, 0));\n"
		" vec4 tex4 = texture2D(texture, TexCoordOut + vec2(.0046875, 0));\n"
		" float Y1 = dot(conversion_factor, vec4(tex1.rgb, 1.0));\n"
		" float Y2 = dot(conversion_factor, vec4(tex2.rgb, 1.0));\n"
		" float Y3 = dot(conversion_factor, vec4(tex3.rgb, 1.0));\n"
		" float Y4 = dot(conversion_factor, vec4(tex4.rgb, 1.0));\n"
		" gl_FragColor = vec4(Y1, Y2, Y3, Y4);\n"
		"}															\n";

	const GLchar *fShaderStrV = 
		"precision highp float; \n"
		"varying vec2 TexCoordOut;\n"
		"uniform sampler2D texture;\n"
		"vec4 conversion_factor = vec4(0.439, -0.368, -0.071, 0.5);\n"
		"void main() \n"
		"{ \n"
		" vec4 tex1 = texture2D(texture, TexCoordOut - vec2(.0046875, 0));\n"
		" vec4 tex2 = texture2D(texture, TexCoordOut - vec2(.0015625, 0));\n"
		" vec4 tex3 = texture2D(texture, TexCoordOut + vec2(.0015625, 0));\n"
		" vec4 tex4 = texture2D(texture, TexCoordOut + vec2(.0046875, 0));\n"
		" float Y1 = dot(conversion_factor, vec4(tex1.rgb, 1.0));\n"
		" float Y2 = dot(conversion_factor, vec4(tex2.rgb, 1.0));\n"
		" float Y3 = dot(conversion_factor, vec4(tex3.rgb, 1.0));\n"
		" float Y4 = dot(conversion_factor, vec4(tex4.rgb, 1.0));\n"
		" gl_FragColor = vec4(Y1, Y2, Y3, Y4);\n"
		"}															\n";

	GLuint vertexShader;
	GLuint fragmentShader;
	GLint compiled;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderStr, NULL);


	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) printf("It didn't compile\n");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fShaderStr, NULL);

	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) printf("It didn't compile\n");

	state->render_program = glCreateProgram();

	glAttachShader(state->render_program, vertexShader);
	glAttachShader(state->render_program, fragmentShader);

	glBindAttribLocation(state->render_program, 0, "vPosition");
	glBindAttribLocation(state->render_program, 1, "TexCoordIn");

	glLinkProgram(state->render_program);

	GLint linked;

	state->unif_pmatrix = glGetUniformLocation(state->render_program, "upmatrix");
	glGetError();
	state->unif_mvmatrix = glGetUniformLocation(state->render_program, "umvmatrix");

	state->unif_tex = glGetUniformLocation(state->render_program, "texture");

	glGetError();
	glGetProgramiv(state->render_program, GL_LINK_STATUS, &linked);

	if(!linked) 
	{
		GLint infoLen = 0;
		glGetProgramiv(state->render_program, GL_INFO_LOG_LENGTH, &infoLen);

		if(infoLen > 1)
		{
			char* infoLog = malloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(state->render_program, infoLen, NULL, infoLog);

			printf("%s\n", infoLog);

			free(infoLog);
		}
		glDeleteProgram(state->render_program);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fShaderStrY, NULL);

	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) printf("It didn't compile\n");

	state->y_program = glCreateProgram();

	glAttachShader(state->y_program, vertexShader);
	glAttachShader(state->y_program, fragmentShader);

	glBindAttribLocation(state->y_program, 0, "vPosition");
	glBindAttribLocation(state->y_program, 1, "TexCoordIn");

	glLinkProgram(state->y_program);

	state->y_pmatrix = glGetUniformLocation(state->y_program, "upmatrix");
	glGetError();
	state->y_mvmatrix = glGetUniformLocation(state->y_program, "umvmatrix");

	state->y_tex = glGetUniformLocation(state->y_program, "texture");

	glGetError();
	glGetProgramiv(state->y_program, GL_LINK_STATUS, &linked);

	if(!linked) 
	{
		GLint infoLen = 0;
		glGetProgramiv(state->y_program, GL_INFO_LOG_LENGTH, &infoLen);

		if(infoLen > 1)
		{
			char* infoLog = malloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(state->y_program, infoLen, NULL, infoLog);

			printf("%s\n", infoLog);

			free(infoLog);
		}
		glDeleteProgram(state->y_program);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fShaderStrU, NULL);

	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) printf("It didn't compile\n");

	state->u_program = glCreateProgram();

	glAttachShader(state->u_program, vertexShader);
	glAttachShader(state->u_program, fragmentShader);

	glBindAttribLocation(state->u_program, 0, "vPosition");
	glBindAttribLocation(state->u_program, 1, "TexCoordIn");

	glLinkProgram(state->u_program);

	state->u_pmatrix = glGetUniformLocation(state->u_program, "upmatrix");
	glGetError();
	state->u_mvmatrix = glGetUniformLocation(state->u_program, "umvmatrix");

	state->u_tex = glGetUniformLocation(state->u_program, "texture");

	glGetError();
	glGetProgramiv(state->u_program, GL_LINK_STATUS, &linked);

	if(!linked) 
	{
		GLint infoLen = 0;
		glGetProgramiv(state->u_program, GL_INFO_LOG_LENGTH, &infoLen);

		if(infoLen > 1)
		{
			char* infoLog = malloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(state->u_program, infoLen, NULL, infoLog);

			printf("%s\n", infoLog);

			free(infoLog);
		}
		glDeleteProgram(state->u_program);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fShaderStrV, NULL);

	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) printf("It didn't compile\n");

	state->v_program = glCreateProgram();

	glAttachShader(state->v_program, vertexShader);
	glAttachShader(state->v_program, fragmentShader);

	glBindAttribLocation(state->v_program, 0, "vPosition");
	glBindAttribLocation(state->v_program, 1, "TexCoordIn");

	glLinkProgram(state->v_program);

	state->v_pmatrix = glGetUniformLocation(state->v_program, "upmatrix");
	glGetError();
	state->v_mvmatrix = glGetUniformLocation(state->v_program, "umvmatrix");

	state->v_tex = glGetUniformLocation(state->v_program, "texture");

	glGetError();
	glGetProgramiv(state->v_program, GL_LINK_STATUS, &linked);

	if(!linked) 
	{
		GLint infoLen = 0;
		glGetProgramiv(state->v_program, GL_INFO_LOG_LENGTH, &infoLen);

		if(infoLen > 1)
		{
			char* infoLog = malloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(state->v_program, infoLen, NULL, infoLog);

			printf("%s\n", infoLog);

			free(infoLog);
		}
		glDeleteProgram(state->v_program);
	}
}

void init_framebuffer(CUBE_STATE_T *state)
{
	glGenFramebuffers(1, &state->offscreen_renderbuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, state->offscreen_renderbuffer);

	glGenTextures(1, &state->render_texture);

	glBindTexture(GL_TEXTURE_2D, state->render_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 640, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, state->render_texture, 0);

	GLuint depthRenderbuffer;

	glGenRenderbuffers(1, &depthRenderbuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 640, 640);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("incomplete framebuffer %u\n", status);
	}

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glGenFramebuffers(1, &state->y_framebuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, state->y_framebuffer);

	GLuint texture;

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 160, 640, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glGenRenderbuffers(1, &depthRenderbuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 160, 640);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("incomplete framebuffer %u\n", status);
	}

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glGenFramebuffers(1, &state->u_framebuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, state->u_framebuffer);

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 80, 320, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glGenRenderbuffers(1, &depthRenderbuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 80, 320);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("incomplete framebuffer %u\n", status);
	}

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glGenFramebuffers(1, &state->v_framebuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, state->v_framebuffer);

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 80, 320, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glGenRenderbuffers(1, &depthRenderbuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 80, 320);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("incomplete framebuffer %u\n", status);
	}

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void init_textures(CUBE_STATE_T *state)
{
	// the texture containing the video
	glGenTextures(1, &state->tex);

	glGetError();
	glBindTexture(GL_TEXTURE_2D, state->tex);
	glGetError();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, state->screen_width,
			state->screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Create EGL Image */
	state->eglImage = eglCreateImageKHR(
			state->display,
			state->context,
			EGL_GL_TEXTURE_2D_KHR,
			(EGLClientBuffer)state->tex,
			0);

	if (state->eglImage == EGL_NO_IMAGE_KHR)
	{
		printf("eglCreateImageKHR failed.\n");
		exit(1);
	}

	// Start rendering
	pthread_create(&thread1, NULL, video_decode_test, state);

	glBindTexture(GL_TEXTURE_2D, state->tex);
}

void create_perspective_matrix(struct matrix *out, float fovy, float aspect,
		float zn, float zf)
{
	identity(out);

	float sine, cosine, cotangent, deltaZ;
	GLfloat radians = fovy / 2.0 * M_PI / 180.0;

	deltaZ = zf - zn;
	sine = sinf(radians);
	cosine = cosf(radians);

	cotangent = cosine / sine;

	out->elements[0][0] = cotangent / aspect;
	out->elements[1][1] = cotangent;
	out->elements[2][2] = -(zf + zn) / deltaZ;
	out->elements[2][3] = -1;
	out->elements[3][2] = -2 * zn * zf / deltaZ;
	out->elements[3][3] = 0;
}

void rotate_matrix(struct matrix *mat, float angle, float x, float y, float z)
{
	float s, c;

	angle = 2.0f * M_PI * angle / 360.0f;

	s = sinf(angle);
	c = cosf(angle);

	struct matrix rotator, temp;

	memcpy(&temp, mat, sizeof(temp)); 

	memset(rotator.elements, 0, 16 * sizeof(GLfloat));

	rotator.elements[0][0] = x * x * (1 - c) + c;
	rotator.elements[1][0] = y * x * (1 - c) + z * s;
	rotator.elements[2][0] = x * z * (1 - c) - y * s;
	rotator.elements[3][0] = 0;

	rotator.elements[0][1] = x * y * (1 - c) - z * s;
	rotator.elements[1][1] = y * y * (1 - c) + c;
	rotator.elements[2][1] = y * z * (1 - c) + x * s;
	rotator.elements[3][1] = 0;

	rotator.elements[0][2] = x * z * (1 - c) + y * s;
	rotator.elements[1][2] = y * z * (1 - c) - x * s;
	rotator.elements[2][2] = z * z * (1 - c) + c;
	rotator.elements[3][2] = 0;

	rotator.elements[0][3] = 0.0;
	rotator.elements[1][3] = 0.0;
	rotator.elements[2][3] = 0.0;
	rotator.elements[3][3] = 1.0;

	mat_mult(mat, &rotator, &temp);
}

void translate_matrix(struct matrix *mat, float x, float y, float z)
{
	struct matrix translator, temp;

	memcpy(&temp, mat, sizeof(temp));

	translator.elements[0][0] = 1.0;
	translator.elements[1][0] = 0.0;
	translator.elements[2][0] = 0.0;
	translator.elements[3][0] = x;

	translator.elements[0][1] = 0.0;
	translator.elements[1][1] = 1.0;
	translator.elements[2][1] = 0.0;
	translator.elements[3][1] = y;

	translator.elements[0][2] = 0.0;
	translator.elements[1][2] = 0.0;
	translator.elements[2][2] = 1.0;
	translator.elements[3][2] = z;

	translator.elements[0][3] = 0.0;
	translator.elements[1][3] = 0.0;
	translator.elements[2][3] = 0.0;
	translator.elements[3][3] = 1.0;

	mat_mult(mat, &translator, &temp);
}

void identity(struct matrix *mat)
{
	int i, j;

	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 4; i++)
		{
			if (i == j)
			{
				mat->elements[i][j] = 1.0;
			}
			else
			{
				mat->elements[i][j] = 0.0;
			}
		}
	}
}

void redraw_scene(CUBE_STATE_T *state)
{
	// Start with a clear screen

	glBindFramebuffer(GL_FRAMEBUFFER, state->offscreen_renderbuffer);

	glViewport(0, 0, 640, 640);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glUseProgram(state->render_program);

	glBindTexture(GL_TEXTURE_2D, state->tex);

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, quadx );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, texCoords );
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glUniformMatrix4fv(state->unif_pmatrix, 1, GL_FALSE, (GLfloat *)&state->p_matrix.elements); 


	rotate_matrix(&state->mv_matrix, state->roll, 0, 0, 1.0);

	glUniformMatrix4fv(state->unif_mvmatrix, 1, GL_FALSE, (GLfloat *)&state->mv_matrix.elements); 

	glUniform1i(state->unif_tex, 0);

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4);

	glGetError();

	glBindFramebuffer(GL_FRAMEBUFFER, state->y_framebuffer);

	glViewport(0, 0, 160, 640);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glUseProgram(state->y_program);

	glBindTexture(GL_TEXTURE_2D, state->render_texture);

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, quadx );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, texCoords );
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glUniformMatrix4fv(state->y_pmatrix, 1, GL_FALSE, (GLfloat *)&state->p_matrix.elements); 

	glUniformMatrix4fv(state->y_mvmatrix, 1, GL_FALSE, (GLfloat *)&state->send_mv_matrix.elements); 

	glUniform1i(state->y_tex, 0);

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4);

	glGetError();

	GLubyte output_frame[160 * 640 * 4 + 2 * (80 * 320 * 4)];

	glReadPixels(0, 0, 160, 640, GL_RGBA, GL_UNSIGNED_BYTE, state->write_buffer);

	glBindFramebuffer(GL_FRAMEBUFFER, state->u_framebuffer);

	glViewport(0, 0, 80, 320);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glUseProgram(state->u_program);

	glBindTexture(GL_TEXTURE_2D, state->render_texture);

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, quadx );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, texCoords );
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glUniformMatrix4fv(state->u_pmatrix, 1, GL_FALSE, (GLfloat *)&state->p_matrix.elements); 

	glUniformMatrix4fv(state->u_mvmatrix, 1, GL_FALSE, (GLfloat *)&state->send_mv_matrix.elements); 

	glUniform1i(state->u_tex, 0);

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4);

	glGetError();

	glReadPixels(0, 0, 80, 320, GL_RGBA, GL_UNSIGNED_BYTE, state->write_buffer + 409600);

	glBindFramebuffer(GL_FRAMEBUFFER, state->v_framebuffer);

	glViewport(0, 0, 80, 320);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glUseProgram(state->v_program);

	glBindTexture(GL_TEXTURE_2D, state->render_texture);

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, quadx );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, texCoords );
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glUniformMatrix4fv(state->v_pmatrix, 1, GL_FALSE, (GLfloat *)&state->p_matrix.elements); 

	glUniformMatrix4fv(state->v_mvmatrix, 1, GL_FALSE, (GLfloat *)&state->send_mv_matrix.elements); 

	glUniform1i(state->v_tex, 0);

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4);

	glGetError();

	glReadPixels(0, 0, 80, 320, GL_RGBA, GL_UNSIGNED_BYTE, state->write_buffer + 512000);

	//	 if (state->dump_frame)
	//	 fprintf(stderr,"%u, %u\n", output_frame[460960], output_frame[563360]);

	/*	 memset(&output_frame[203518], 29, 6);
			 memset(&output_frame[205758], 29, 6);
			 memset(&output_frame[206398], 29, 6);
			 memset(&output_frame[207038], 29, 6);
			 memset(&output_frame[207678], 29, 6);
			 memset(&output_frame[208318], 29, 6);

			 memset(&output_frame[460956], 255, 3);
			 memset(&output_frame[461278], 255, 3);
			 memset(&output_frame[461598], 255, 3);

			 memset(&output_frame[563998], 107, 3);
			 memset(&output_frame[564318], 107, 3);
			 memset(&output_frame[564638], 107, 3);*/

	//	 fwrite(state->write_buffer, 1, sizeof(output_frame), stdout);
	//		printf("%u, %u, %u, %u\n", output_frame[8], output_frame[9], output_frame[10], output_frame[11]);


	// glBindFramebuffer(GL_FRAMEBUFFER	, 0);
}

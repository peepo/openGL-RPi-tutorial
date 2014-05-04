add_executable(tutorial05_gen_YUV_tex_disp
    tutorial05.cpp
)
target_link_libraries(tutorial05_gen_YUV_tex_disp
        common
        ${RPi_LIBS}
        ${GL_LIBS}
        ${CAM_LIBS}
)
file(
	COPY
	simplefragshader.glsl
	simplevertshader.glsl
	yuvfragshader.glsl
	DESTINATION ${CMAKE_BINARY_DIR}/tutorial05_gen_YUV_tex_disp
)

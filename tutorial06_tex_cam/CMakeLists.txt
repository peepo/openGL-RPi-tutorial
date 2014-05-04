# Tutorial 6 use camera as texture Chris Cummings
add_executable(tutorial06_tex_cam
    tutorial06.cpp
    ../common/graphics.cpp
)
target_link_libraries(tutorial06_tex_cam
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
	DESTINATION ${CMAKE_BINARY_DIR}/tutorial06_tex_cam
)

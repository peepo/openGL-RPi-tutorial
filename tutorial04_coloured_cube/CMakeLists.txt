add_executable(tutorial04_coloured_cube
    tutorial04.cpp
)
target_link_libraries(tutorial04_coloured_cube
        common
        ${RPi_LIBS}
        ${GL_LIBS}
)
file(
	COPY
	colourfragshader.glsl
	transformvertshader.glsl
	DESTINATION ${CMAKE_BINARY_DIR}/tutorial04_coloured_cube
)

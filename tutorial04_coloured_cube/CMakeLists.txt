add_executable(tutorial04_coloured_cube
    tutorial04.cpp
    ../common/startScreen.cpp
    ../common/LoadShaders.cpp
    ../common/LoadShaders.h
)
target_link_libraries(tutorial04_coloured_cube
        ${RPi_LIBS}
        ${GL_LIBS}
)
file(
	COPY
	colourfragshader.glsl
	transformvertshader.glsl
	DESTINATION ${CMAKE_BINARY_DIR}/tutorial04_coloured_cube
)

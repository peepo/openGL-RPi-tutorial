add_executable(tutorial02a_modelspace
    tutorial02a.cpp
)
target_link_libraries(tutorial02a_modelspace
        common
        ${RPi_LIBS}
        ${GL_LIBS}
)
file(
	COPY
	simplefragshader.glsl
	simplevertshader.glsl
	DESTINATION ${CMAKE_BINARY_DIR}/tutorial02a_modelspace
)

add_executable(tutorial02_red_triangle
    tutorial02.cpp
    )
target_link_libraries(tutorial02_red_triangle
        common
        ${RPi_LIBS}
        ${GL_LIBS}
)
file(
	COPY
	simplefragshader.glsl
	simplevertshader.glsl
	DESTINATION ${CMAKE_BINARY_DIR}/tutorial02_red_triangle
)

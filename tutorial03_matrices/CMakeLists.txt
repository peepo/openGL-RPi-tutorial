add_executable(tutorial03_matrices
    tutorial03.cpp
)
target_link_libraries(tutorial03_matrices
        common
        ${RPi_LIBS}
        ${GL_LIBS}
)
file(
	COPY
	simplefragshader.glsl
	simpletransformvertshader.glsl
	DESTINATION ${CMAKE_BINARY_DIR}/tutorial03_matrices
)

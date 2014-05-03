add_executable(tutorial03_matrices
    tutorial03.cpp
    ../common/startScreen.cpp
    ../common/LoadShaders.cpp
    ../common/LoadShaders.h
)
target_link_libraries(tutorial03_matrices
        ${RPi_LIBS}
        ${GL_LIBS}
)
file(
	COPY
	simplefragshader.glsl
	simpletransformvertshader.glsl
	DESTINATION ${CMAKE_BINARY_DIR}/tutorial03_matrices
)

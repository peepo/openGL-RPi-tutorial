add_executable(tutorial02a_modelspace
    tutorial02a.cpp
    ../common/startScreen.cpp
    ../common/LoadShaders.cpp
    ../common/LoadShaders.h
)
target_link_libraries(tutorial02a_modelspace
        ${RPi_LIBS}
        ${GL_LIBS}
)
file(
	COPY
	simplefragshader.glsl
	simplevertshader.glsl
	DESTINATION ${CMAKE_BINARY_DIR}/tutorial02a_modelspace
)

add_executable(tutorial05_tex
    tutorial05.cpp
)
target_link_libraries(tutorial05_tex
        common
        ${RPi_LIBS}
        ${GL_LIBS}
)
file(
	COPY
	simplevertshader.glsl
	texturefragshader.glsl
	uvtemplate.bmp
	DESTINATION ${CMAKE_BINARY_DIR}/tutorial05_tex
)

add_executable(tutorial05_textured_cube
    tutorial05.cpp
)
target_link_libraries(tutorial05_textured_cube
        common
        ${RPi_LIBS}
        ${GL_LIBS}
)
file(
	COPY
	transformvertshader.glsl
	texturefragshader.glsl
	uvtemplate.bmp
	uvtemplate.DDS
	uvtemplate.tga
	DESTINATION ${CMAKE_BINARY_DIR}/tutorial05_textured_cube
)

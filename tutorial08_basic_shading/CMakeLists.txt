# Tutorial 8 basic shading
add_executable(tutorial08_basic_shading
    tutorial08.cpp
)
target_link_libraries(tutorial08_basic_shading
    common
    ${RPi_LIBS}
    ${GL_LIBS}
)
file(
COPY
suzanne.obj
TextureFragmentShader.glsl
TransformVertexShader.glsl
uvtemplate.bmp
DESTINATION ${CMAKE_BINARY_DIR}/tutorial08_basic_shading
)

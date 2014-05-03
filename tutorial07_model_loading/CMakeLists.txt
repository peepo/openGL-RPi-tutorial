# Tutorial 7 obj model loader
add_executable(tutorial07_model_loading
    tutorial07.cpp
)
target_link_libraries(tutorial07_model_loading
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
DESTINATION ${CMAKE_BINARY_DIR}/tutorial07_model_loading
)

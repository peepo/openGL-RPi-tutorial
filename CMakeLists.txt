# CMake entry point
cmake_minimum_required (VERSION 2.8)
project ("openGLES 2.0 for Raspberry Pi tutorials")

SET(COMPILE_DEFINITIONS -Werror)

include_directories(
    /opt/vc/include
    /opt/vc/include/interface/vcos/pthreads
    /opt/vc/include/interface/vmcs_host/linux
<<<<<<< HEAD
=======
    ${CMAKE_SOURCE_DIR}/libs/glm/
    ${CMAKE_SOURCE_DIR}/libs/ilclient
>>>>>>> d40b8e62d2151c51ed796c21e32be4903d18f949
)

set(RPi_LIBS
# Raspberry Pi - Broadcom
    libbcm_host.so
    libvcos.so
)

set(CAM_LIBS
# camera
    libmmal_core.so
    libmmal_util.so
    libmmal_vc_client.so
)

set(GL_LIBS
# openGLES
    GLESv2
    EGL
)

set(ILC_LIBS
# ilclient
    ilclient
    openmaxil
)

<<<<<<< HEAD
link_directories(../libs/ilclient /opt/vc/lib)

# Tutorial 1
add_executable(../tutorial01_first_screen/tutorial01_first_screen
    tutorial01_first_screen/tutorial01.cpp
    common/startScreen.cpp
)
target_link_libraries(../tutorial01_first_screen/tutorial01_first_screen
        ${RPi_LIBS}
        ${GL_LIBS}
)

# playground
add_executable(../playground/playground
    playground/playground.cpp
    common/startScreen.cpp
)
target_link_libraries(../playground/playground
        ${RPi_LIBS}
        ${GL_LIBS}
)


# Tutorial 2
add_executable(../tutorial02_red_triangle/tutorial02_red_triangle
    tutorial02_red_triangle/tutorial02.cpp
    common/startScreen.cpp
    common/LoadShaders.cpp
    common/LoadShaders.h
)
target_link_libraries(../tutorial02_red_triangle/tutorial02_red_triangle
        ${RPi_LIBS}
        ${GL_LIBS}
)

# Tutorial 2a
add_executable(../tutorial02a_modelspace/tutorial02a_modelspace
    tutorial02a_modelspace/tutorial02a.cpp
    common/startScreen.cpp
    common/LoadShaders.cpp
    common/LoadShaders.h
)
target_link_libraries(../tutorial02a_modelspace/tutorial02a_modelspace
        ${RPi_LIBS}
        ${GL_LIBS}
)

# Tutorial 3
add_executable(../tutorial03_matrices/tutorial03_matrices
    tutorial03_matrices/tutorial03.cpp
    common/startScreen.cpp
    common/LoadShaders.cpp
    common/LoadShaders.h
)
target_link_libraries(../tutorial03_matrices/tutorial03_matrices
        ${RPi_LIBS}
        ${GL_LIBS}
)

# Tutorial 4
add_executable(../tutorial04_coloured_cube/tutorial04_coloured_cube
    tutorial04_coloured_cube/tutorial04.cpp
    common/startScreen.cpp
    common/LoadShaders.cpp
    common/LoadShaders.h
)
target_link_libraries(../tutorial04_coloured_cube/tutorial04_coloured_cube
        ${RPi_LIBS}
        ${GL_LIBS}
)

# Tutorial 5 use .bmp as texture, simplified
add_executable(../tutorial05_tex/tutorial05_tex
    tutorial05_tex/tutorial05.cpp
    common/startScreen.cpp
    common/LoadShaders.cpp
    common/LoadShaders.h
    common/texture.cpp
    common/texture.h
)
target_link_libraries(../tutorial05_tex/tutorial05_tex
        ${RPi_LIBS}
        ${GL_LIBS}
)

# Tutorial 5 use .bmp as texture on 3D cube
add_executable(../tutorial05_textured_cube/tutorial05_textured_cube
    tutorial05_textured_cube/tutorial05.cpp
    common/startScreen.cpp
    common/LoadShaders.cpp
    common/LoadShaders.h
    common/texture.cpp
    common/texture.h
)
target_link_libraries(../tutorial05_textured_cube/tutorial05_textured_cube
        ${RPi_LIBS}
        ${GL_LIBS}
)


# Tutorial 5 generate YUV texture
add_executable(../tutorial05_gen_YUV_tex_disp/tutorial05_gen_YUV_tex_disp
    tutorial05_gen_YUV_tex_disp/tutorial05.cpp
    common/camera.cpp
    common/cameracontrol.cpp
    common/graphics.cpp
)
target_link_libraries(../tutorial05_gen_YUV_tex_disp/tutorial05_gen_YUV_tex_disp
        ${RPi_LIBS}
        ${GL_LIBS}
        ${CAM_LIBS}
)

# Tutorial 6 use camera as texture Chris Cummings
add_executable(../tutorial06_tex_cam/tutorial06_tex_cam
    tutorial06_tex_cam/tutorial06.cpp
    common/camera.cpp
    common/cameracontrol.cpp
    common/graphics.cpp
    #common/startScreen.cpp
    #common/LoadShaders.cpp
    #common/LoadShaders.h
)
target_link_libraries(../tutorial06_tex_cam/tutorial06_tex_cam
        ${RPi_LIBS}
        ${GL_LIBS}
        ${CAM_LIBS}
)

# Tutorial 7 obj model loader
add_executable(../tutorial07_model_loading/tutorial07_model_loading
    tutorial07_model_loading/tutorial07.cpp
    common/objloader.cpp
    common/startScreen.cpp
    common/LoadShaders.cpp
    common/texture.cpp
)
target_link_libraries(../tutorial07_model_loading/tutorial07_model_loading
        ${RPi_LIBS}
        ${GL_LIBS}
)

# Tutorial encode YUV
add_executable(../encode/encode
    encode/encode.c
)

target_link_libraries(../encode/encode
        ${RPi_LIBS}
        ${ILC_LIBS}
)
set (CMAKE_C_FLAGS "-DOMX_SKIP64BIT")

# Tutorial encode video using main and header files
add_executable(../encode_main/encode
    encode_main/encode.c
    encode_main/main.c
)

target_link_libraries(../encode_main/encode
        ${RPi_LIBS}
        ${ILC_LIBS}
)
set (CMAKE_C_FLAGS "-DOMX_SKIP64BIT")

# Tutorial decode camera YUV video, transform to RGBA, apply matrix in OGL, then transform to YUV and encode to .h264
add_executable(../encode_OGL/encode
    encode_OGL/src/encode.c
    encode_OGL/src/video.c
    encode_OGL/src/matrix.c
    encode_OGL/src/ogl.c
    encode_OGL/main.c
)

target_link_libraries(../encode_OGL/encode
        ${RPi_LIBS}
        ${GL_LIBS}
        ${CAM_LIBS}
        ${ILC_LIBS}
)
set (CMAKE_C_FLAGS "-DOMX_SKIP64BIT")

# Tutorial 7
=======
link_directories(${CMAKE_SOURCE_DIR}/libs/ilclient /opt/vc/lib)

# subdirectories
add_subdirectory(playground)
add_subdirectory(tutorial01_first_screen)
add_subdirectory(tutorial02_red_triangle)
add_subdirectory(tutorial02a_modelspace)
add_subdirectory(tutorial03_matrices)
add_subdirectory(tutorial04_coloured_cube)
add_subdirectory(tutorial05_tex)
add_subdirectory(tutorial05_textured_cube)
add_subdirectory(tutorial05_gen_YUV_tex_disp)
add_subdirectory(tutorial06_tex_cam)
add_subdirectory(encode)
add_subdirectory(encode_main)
add_subdirectory(encode_OGL)
>>>>>>> d40b8e62d2151c51ed796c21e32be4903d18f949

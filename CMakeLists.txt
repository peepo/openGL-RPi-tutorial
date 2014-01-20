# CMake entry point
cmake_minimum_required (VERSION 2.8)
project ("openGLES 2.0 for Raspberry Pi tutorials")

SET(COMPILE_DEFINITIONS -Werror)

include_directories(
    /opt/vc/include
    /opt/vc/include/interface/vcos/pthreads
    /opt/vc/include/interface/vmcs_host/linux
    /opt/vc/userland
    /opt/vc/userland/interface/vcos
    /opt/vc/userland/interface/vcos/pthreads
    /opt/vc/userland/interface/vmcs_host/linux
    /opt/vc/userland/host_applications/linux/libs/bcm_host/include
    ${CMAKE_SOURCE_DIR}/libs/glm/
    ${CMAKE_SOURCE_DIR}/libs/ilclient
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

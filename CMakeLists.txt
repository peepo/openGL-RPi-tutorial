# CMake entry point
cmake_minimum_required (VERSION 2.8)
project ("openGLES 2.0 for Raspberry Pi tutorials")

SET(COMPILE_DEFINITIONS -Werror)

 if (CROSS_COMPILE)
    message("Cross compilation mode enabled.")

    # Some cmake systems has bug, and uses install rpath
    # always, even while building
    # (CMAKE_BUILD_RPATH is always equal to CMAKE_INSTALL_RPATH).
    # In cross-compilation case such rpath would be wrong.
    # E.g. we don't need rpath=/opt/vc/lib
    # but we need rpath=${ROOTFS}/opt/vc/lib
    # Currently the we just hardcode that linker flag.
    SET(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath,${CMAKE_SOURCE_DIR}/libs/ilclient:${CMAKE_SYSROOT}/opt/vc/lib")
endif(CROSS_COMPILE)

include_directories(
    ${CMAKE_SYSROOT}/opt/vc/include
    ${CMAKE_SYSROOT}/opt/vc/include/interface/vcos/pthreads
    ${CMAKE_SYSROOT}/opt/vc/include/interface/vmcs_host/linux
#   ${CMAKE_SOURCE_DIR}/libs/glm/
    ${CMAKE_SOURCE_DIR}/libs/ilclient
    ${CMAKE_SOURCE_DIR}/common
)

link_directories(
    ${CMAKE_SOURCE_DIR}/libs/ilclient
    ${CMAKE_SYSROOT}/opt/vc/lib
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


# common library
add_subdirectory(common)

# subdirectories
add_subdirectory(encode)
add_subdirectory(encode_main)
add_subdirectory(encode_OGL)
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
add_subdirectory(tutorial07_model_loading)
add_subdirectory(tutorial08_basic_shading)

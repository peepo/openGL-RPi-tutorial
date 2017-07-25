# Tutorial decode camera YUV video, transform to RGBA, apply matrix in OGL, then transform to YUV and encode to .h264
add_executable(encode_OGL
    src/encode.c
    src/video.c
    src/matrix.c
    src/ogl.c
    main.c
)

target_link_libraries(encode_OGL
        ${RPi_LIBS}
        ${GL_LIBS}
        ${CAM_LIBS}
        ${ILC_LIBS}
        m
        pthread
)
set (CMAKE_C_FLAGS "-DOMX_SKIP64BIT")

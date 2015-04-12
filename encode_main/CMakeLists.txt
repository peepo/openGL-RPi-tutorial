# Tutorial encode video using main and header files
add_executable(encode_main
    encode.c
    main.c
)
target_link_libraries(encode_main
        ${RPi_LIBS}
        ${ILC_LIBS}
)
set (CMAKE_C_FLAGS "-DOMX_SKIP64BIT -lpthread")

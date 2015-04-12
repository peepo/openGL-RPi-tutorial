add_executable(encode
    encode.c
)
target_link_libraries(encode
        ${RPi_LIBS}
        ${ILC_LIBS}
)
set (CMAKE_C_FLAGS "-DOMX_SKIP64BIT -lpthread")

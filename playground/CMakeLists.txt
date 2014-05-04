add_executable(playground
    playground.cpp
)
target_link_libraries(playground
        common
        ${RPi_LIBS}
        ${GL_LIBS}
)

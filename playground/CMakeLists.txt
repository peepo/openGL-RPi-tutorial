add_executable(playground
    playground.cpp
    ../common/startScreen.cpp
)
target_link_libraries(playground
        ${RPi_LIBS}
        ${GL_LIBS}
)

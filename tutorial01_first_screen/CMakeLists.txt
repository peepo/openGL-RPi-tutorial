add_executable(tutorial01_first_screen
    tutorial01.cpp
    ../common/startScreen.cpp
)
target_link_libraries(tutorial01_first_screen
        ${RPi_LIBS}
        ${GL_LIBS}
)

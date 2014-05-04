add_executable(tutorial01_first_screen
    tutorial01.cpp
)
target_link_libraries(tutorial01_first_screen
        common
        ${RPi_LIBS}
        ${GL_LIBS}
)

SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)

# Setup rootfs directory here
SET(PIROOT  /home/stepan/projects/alpha_racing/raspberry-rootfs/rootfs)

# Directory to raspberry cross compiling tools
SET(PITOOLS /opt/tools)

SET(TOOLROOT ${PITOOLS}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64)

# specify the cross compiler
SET(CMAKE_C_COMPILER   ${TOOLROOT}/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLROOT}/bin/arm-linux-gnueabihf-g++)

SET(CMAKE_SYSROOT ${PIROOT})
SET(CMAKE_FIND_ROOT_PATH ${PIROOT})


# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# Some cmake systems has bug, and uses install rpath
# always, even while building.
# In cross-compilation case such rpath would be wrong.
# E.g. we don't need rpath=/opt/vc/lib
# but we need rpath=${ROOTFS}/opt/vc/lib
# Currently we should hardcode that linker flag in project CMakeLists.txt
SET(CMAKE_SKIP_BUILD_RPATH True)

set(CROSS_COMPILE True)


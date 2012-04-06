cmake_minimum_required(VERSION 2.8)
project(gfxsandbox)

# Need additional CMake modules not included in standard dist
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_DEBUG_POSTFIX "d")

# Enable clang's C++0x support on the mac
set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -stdlib=libc++ -g -Wall")

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
set(srcs
    src/gfxsandbox.cpp
    src/glutil.cpp
    src/util.cpp
    src/shader.cpp
    src/texture.cpp
)

set(headers
    src/util.h
)

set(CMAKE_CXX_FLAGS "-g -Wall -Werror")
add_subdirectory(content)

add_executable(gfxsandbox ${srcs} ${headers})
include_directories(
    src
    ${GLUT_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
)
target_link_libraries(
    gfxsandbox
    ${GLUT_LIBRARY}
    ${OPENGL_LIBRARY}
    ${GLEW_LIBRARY})

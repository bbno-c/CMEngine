cmake_minimum_required(VERSION 3.23.1)

if(${CMAKE_VERSION} VERSION_LESS 3.25)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.25)
endif()

project(glew-static)

set(SOURCE 
    src/glew.c
)

set(INCLUDE 
    include/GL/glew.h
    include/GL/wglew.h
)

add_library(glew-static STATIC ${SOURCE} ${INCLUDE})

target_compile_definitions(glew-static 
PUBLIC
    GLEW_STATIC
    GLEW_NO_GLU
)

target_include_directories(glew-static 
PUBLIC
    include
)

add_library(GLEW::glew ALIAS glew-static)


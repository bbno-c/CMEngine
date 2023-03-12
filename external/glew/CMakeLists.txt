cmake_minimum_required(VERSION 3.23.1)

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


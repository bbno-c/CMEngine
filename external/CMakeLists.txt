cmake_minimum_required(VERSION 3.23.1)

project(external_lib)

set(EXTERNAL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB GLAD_SOURCE "glad/src/*.cpp" "glad/src/*.c" "glad/src/*.cc")

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME} 
    external.cpp
    ${GLAD_SOURCE}
)

add_subdirectory(SDL)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC SDL3::SDL3
    PUBLIC opengl32
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${EXTERNAL_LIB_PATH}/glad/include"
)
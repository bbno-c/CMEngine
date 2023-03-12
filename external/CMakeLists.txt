cmake_minimum_required(VERSION 3.23.1)

project(external_lib)

set(EXTERNAL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME} 
    external.cpp
)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(SDL_STATIC ON)
set(SDL_SHARED OFF)
add_subdirectory(SDL)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC SDL3::SDL3
)

add_subdirectory(glew)
target_link_libraries(${PROJECT_NAME} 
    PUBLIC glew-static
)

if(PLATFORM_WIN32)
target_link_libraries(${PROJECT_NAME} PUBLIC opengl32)
elseif(PLATFORM_LINUX)
elseif(PLATFORM_MACOS)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME}  PUBLIC ${OPENGL_LIBRARY})
elseif(PLATFORM_ANDROID)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()
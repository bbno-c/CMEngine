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
)

if(PLATFORM_WIN32)
target_link_libraries(${PROJECT_NAME}  PRIVATE opengl32)
elseif(PLATFORM_LINUX)
elseif(PLATFORM_MACOS)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME}  PRIVATE ${OPENGL_LIBRARY})
elseif(PLATFORM_ANDROID)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${EXTERNAL_LIB_PATH}/glad/include"
)
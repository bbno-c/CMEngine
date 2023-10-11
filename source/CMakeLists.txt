cmake_minimum_required(VERSION 3.23.1)

if(${CMAKE_VERSION} VERSION_LESS 3.25)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.25)
endif()

project(source)

set(ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(EXTERNALS_PATH "${ENGINE_ROOT}/../external")

add_subdirectory(../external CMAKE_RUNTIME_OUTPUT_DIRECTORY)

file(GLOB_RECURSE ENGINE_UTILITY_SOURCE "utility/*.cpp" "utility/*.c" "utility/*.cc" "utility/*.h" "utility/*.hpp")
file(GLOB_RECURSE ENGINE_SYSTEM_SOURCE "system/*.cpp" "system/*.c" "system/*.cc" "system/*.h" "system/*.hpp")
file(GLOB_RECURSE ENGINE_FILESYSTEM_SOURCE "FileSystem/*.cpp" "FileSystem/*.c" "FileSystem/*.cc" "FileSystem/*.h" "FileSystem/*.hpp")
file(GLOB_RECURSE ENGINE_GRAPHICS_SOURCE "graphics/*.cpp" "graphics/*.c" "graphics/*.cc" "graphics/*.h" "graphics/*.hpp")
file(GLOB ENGINE_SOURCE "*.cpp" "*.c" "*.cc" "*.h" "*.hpp")

add_library(${PROJECT_NAME} STATIC 
${ENGINE_UTILITY_SOURCE}
${ENGINE_SYSTEM_SOURCE}
${ENGINE_GRAPHICS_SOURCE}
${ENGINE_SOURCE}
${ENGINE_FILESYSTEM_SOURCE}
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC external_lib
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${EXTERNALS_PATH}/SDL/include"
    PUBLIC "${EXTERNALS_PATH}/glew/include/GL"
    PUBLIC "${EXTERNALS_PATH}/glm"
    PUBLIC "${ENGINE_ROOT}/utility"
    PUBLIC "${ENGINE_ROOT}"
    PUBLIC "${ENGINE_ROOT}/FileSystem"
)

target_precompile_headers( ${PROJECT_NAME} PUBLIC common.h)
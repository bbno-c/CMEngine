cmake_minimum_required(VERSION 3.23.1)

project(source)

set(ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(EXTERNALS_PATH "${ENGINE_ROOT}/../external")

add_subdirectory(../external CMAKE_RUNTIME_OUTPUT_DIRECTORY)

file(GLOB_RECURSE ENGINE_UTILITY_SOURCE "utility/*.cpp" "utility/*.c" "utility/*.cc" "utility/*.h" "utility/*.hpp")
file(GLOB_RECURSE ENGINE_SYSTEM_SOURCE "system/*.cpp" "system/*.c" "system/*.cc" "system/*.h" "system/*.hpp")

add_library(${PROJECT_NAME} STATIC 
${ENGINE_UTILITY_SOURCE}
${ENGINE_SYSTEM_SOURCE}
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC external_lib
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${EXTERNALS_PATH}/SDL/include"
    PUBLIC "${EXTERNALS_PATH}/glmlib"
    PUBLIC "${ENGINE_ROOT}/utility"
)
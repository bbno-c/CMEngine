cmake_minimum_required(VERSION 3.23.1)

if(${CMAKE_VERSION} VERSION_LESS 3.25)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.25)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(MAIN_PROJECT_DIRECTORY_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/source")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/project")
cmake_minimum_required(VERSION 3.23.1)

project(cmengine)

if(ANDROID)
add_library(${PROJECT_NAME} main.cpp)
else()
add_executable(${PROJECT_NAME} "source/entryPoint/main.cpp")
endif()

set(SDL_STATIC ON)
set(SDL_SHARED OFF)
add_subdirectory(external/SDL)
target_link_libraries(${PROJECT_NAME} 
    PUBLIC SDL3::SDL3
)

add_subdirectory(external/glew)
target_link_libraries(${PROJECT_NAME} 
    PUBLIC glew-static
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(WARNING "Linux platform is currently not supported.")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Find OpenGL package
    find_package(OpenGL REQUIRED)

    # Include OpenGL and GLU headers
    include_directories(${OPENGL_INCLUDE_DIR})

    # Link OpenGL and GLU libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES} glu32)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Find OpenGL package
    find_package(OpenGL REQUIRED)

    # Link OpenGL and GLU libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARY} GLU)
endif()

add_subdirectory(external/stb)
target_link_libraries(${PROJECT_NAME} 
    PUBLIC stb_image
)

add_subdirectory(external/libimgui)
target_link_libraries(${PROJECT_NAME} 
    PUBLIC imgui
)


target_compile_definitions(${PROJECT_NAME} PUBLIC
  $<$<CONFIG:Debug>:DEBUG>
  $<$<CONFIG:Release>:RELEASE>
)

if(${CMAKE_VERSION} VERSION_LESS 3.25)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.25)
endif()

#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(MAIN_PROJECT_DIRECTORY_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(EXTERNALS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external")

file(GLOB_RECURSE ENGINE_SOURCE "source/*.cpp" "source/*.c" "source/*.cc" "source/*.h" "source/*.hpp")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_sources(${PROJECT_NAME} PRIVATE 
${ENGINE_SOURCE}
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${EXTERNALS_PATH}/SDL/include"
    PUBLIC "${EXTERNALS_PATH}/glew/include/GL"
    PUBLIC "${EXTERNALS_PATH}/libimgui/imgui"
    PUBLIC "${EXTERNALS_PATH}/glm"
    PUBLIC "${ENGINE_ROOT}/utility"
    PUBLIC "${ENGINE_ROOT}"
    PUBLIC "${ENGINE_ROOT}/FileSystem"
)

target_precompile_headers( ${PROJECT_NAME} PUBLIC source/common.h)
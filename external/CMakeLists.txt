cmake_minimum_required(VERSION 3.23.1)

if(${CMAKE_VERSION} VERSION_LESS 3.25)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.25)
endif()

project(external_lib)

set(EXTERNAL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} 
    external.cpp
)

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


add_subdirectory(stb)
add_subdirectory(libimgui)

#add_library(stb INTERFACE)
#target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb")

#add_library(imgui)
#target_sources(imgui PUBLIC 
#"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"
#"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp"
#"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
#"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp"
#"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
#"${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.cpp"
#"${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
#)
#
#target_include_directories(imgui PUBLIC
#"${CMAKE_CURRENT_SOURCE_DIR}/imgui"     
#"${CMAKE_CURRENT_SOURCE_DIR}/SDL/include"
#)
#target_link_libraries(${PROJECT_NAME} PUBLIC imgui SDL3::SDL3)

#add_subdirectory(libimgui)

cmake_minimum_required(VERSION 3.5)

project(cmengine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(QT_ROOT_DIR         "qt-5.15.3-dynamic-msvc2019-x86_64")
set(QT_DIR              "${QT_ROOT_DIR}/lib/cmake/Qt5")
set(Qt5_DIR             "${QT_ROOT_DIR}/lib/cmake/Qt5")
set(QT_QMAKE_EXECUTABLE "${QT_ROOT_DIR}/bin/qmake.exe")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(MAIN_PROJECT_DIRECTORY_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(EXTERNALS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external")

if(ANDROID)
add_library(${PROJECT_NAME} main.cpp)
else()
add_executable(${PROJECT_NAME} "source/entryPoint/main.cpp" "source/graphics/Window.h" "source/graphics/Window.cpp")
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

# Set the output directories for external projects (SDL, glew, stb, libimgui)
set_target_properties(glew-static stb_image imgui PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
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

# Set the output directories for your main project again
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)


file(GLOB_RECURSE ENGINE_SOURCE "source/*.cpp" "source/*.c" "source/*.cc" "source/*.h" "source/*.hpp")

target_sources(${PROJECT_NAME} PRIVATE 
${ENGINE_SOURCE}
)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${EXTERNALS_PATH}/SDL/include"
    PUBLIC "${EXTERNALS_PATH}/glew/include/GL"
    PUBLIC "${EXTERNALS_PATH}/libimgui/imgui"
    PUBLIC "${EXTERNALS_PATH}/glm"
    PUBLIC "${ENGINE_ROOT}/utility"
    PUBLIC "${ENGINE_ROOT}"
    PUBLIC "${ENGINE_ROOT}/FileSystem"
)

# Set include directories
include_directories(
    ${EXTERNALS_PATH}/assimpdll/include
    ${QTDIR}/include
    ${QTDIR}/include/QtCore
    ${QTDIR}/include/QtGui
    ${QTDIR}/include/QtUiTools
    ${QTDIR}/include/QtWidgets
)
# Set additional library directories
link_directories(
    ${QTDIR}/lib
)
add_compile_options(/D UNICODE /D WIN32 /D WIN64 /D QT_CORE_LIB /D QT_GUI_LIB /D QT_UITOOLS_LIB /D QT_WIDGETS_LIB /MDd)

add_library(Assimp SHARED IMPORTED)
set_target_properties(Assimp PROPERTIES
    IMPORTED_LOCATION "${EXTERNALS_PATH}/assimpdll/lib/assimp-vc143-mt.dll"
    IMPORTED_IMPLIB "${EXTERNALS_PATH}/assimpdll/lib//assimp-vc143-mt.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${EXTERNALS_PATH}/assimpdll/include"
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC Assimp
)

target_precompile_headers( ${PROJECT_NAME} PUBLIC source/common.h)

# Copy necessary DLLs after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QT_ROOT_DIR}/bin/Qt5Widgetsd.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QT_ROOT_DIR}/bin/Qt5Guid.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QT_ROOT_DIR}/bin/Qt5Cored.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${EXTERNALS_PATH}/assimpdll/lib/assimp-vc143-mt.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${EXTERNALS_PATH}/assimpdll/lib/assimp-vc143-mt.lib"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets
)

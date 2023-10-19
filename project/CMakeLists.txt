cmake_minimum_required(VERSION 3.23.1)
project(cmengine)

if(ANDROID)
add_library(${PROJECT_NAME} main.cpp)
else()
add_executable(${PROJECT_NAME} main.cpp)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE source)

target_compile_definitions(${PROJECT_NAME} PUBLIC
  $<$<CONFIG:Debug>:DEBUG>
  $<$<CONFIG:Release>:RELEASE>
)

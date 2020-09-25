cmake_minimum_required(VERSION 3.13)
project(OpenGL)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)

file(COPY "res" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(lib)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/callbacks.cpp
        src/glad.c
        src/shader.cpp
        src/stb_image.cpp)

target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        glfw)

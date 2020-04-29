cmake_minimum_required(VERSION 3.13)
project(OpenGL)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)

if(OPENGL_FOUND)
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
endif()

configure_file(
  res/shaders/basic.glsl ${CMAKE_CURRENT_BINARY_DIR}/res/shaders/basic.glsl COPYONLY
)

configure_file(
  res/shaders/texture.glsl ${CMAKE_CURRENT_BINARY_DIR}/res/shaders/texture.glsl COPYONLY
)

configure_file(
  res/textures/wall.jpg ${CMAKE_CURRENT_BINARY_DIR}/res/textures/wall.jpg COPYONLY
)

configure_file(
  res/textures/container.jpg ${CMAKE_CURRENT_BINARY_DIR}/res/textures/container.jpg COPYONLY
)

configure_file(
  res/textures/awesomeface.png ${CMAKE_CURRENT_BINARY_DIR}/res/textures/awesomeface.png COPYONLY
)

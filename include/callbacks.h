#ifndef OPENGL_CALLBACKS_H
#define OPENGL_CALLBACKS_H

#include <GLFW/glfw3.h>

void processInputs(GLFWwindow *window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif //OPENGL_CALLBACKS_H
